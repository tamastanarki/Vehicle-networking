#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <xil_printf.h>
#include <platform.h>
#include <timers.h>
#include <libuserchannel-riscv.h>
#include "can_phy.h"
#include "vep_private_memory.h"
#include "io.h"
#include "io.c"
#include "settings.h"

#define QUANTUM 1571000
#define SS_LEN  (QUANTUM)
#define PROP_LEN (4 * QUANTUM)
#define PS_1_LEN (4 * QUANTUM)
#define PS_2_LEN (4 * QUANTUM)
#define SJW (1 * QUANTUM)

#define READ_GPIO() ((gpio[GPIO_RW] & 2) == 2 ? RECESSIVE : DOMINANT)


volatile uint32_t *gpio = (uint32_t*)GPIO;
int samples[3];
void transmit_symbol();

int main ( void )
{
  volatile uint64_t t_start = read_global_timer();
  xil_printf("%04u/%010u: CAN PHY driver\n", (uint32_t)(t_start >> 32), (uint32_t)t_start);

  // initialization
  for (unsigned int i = 2; i < (2 + MAX_NUM_CAN_NODES); ++i)
    MEM->transmitting_symbols[i] = UNSPECIFIED;
  MEM->read_symbol = UNSPECIFIED;

  // Set 1 pin as output, rest as input
  gpio[GPIO_CONFIG] = 0xFE;
  // Set output (TX) (bit 1) as recessive
  gpio[GPIO_RW] = RECESSIVE;

  CAN_SYMBOL initial_value, new_value;
  int hard_sync = 0, resync = 0;
  uint64_t ps1_extension = 0;
  uint64_t ps2_shrinkage = 0;
  uint64_t t_tmp, end_of_ss;
  int recessive_cntr = 0;
  CAN_SYMBOL resolved_value;

  if (TILE_ID == 1 && DEBUG_CAN_PHY & DEBUG_CAN_PHY_LED) set_led_color(0, 0, 0, 0);
  while(1) {
    t_start = read_global_timer();
    MEM->current_segment = SYNC_SEG;
    if (TILE_ID == 1 && DEBUG_CAN_PHY & DEBUG_CAN_PHY_CLK) xil_printf("%04u/%010u: SYNC_SEG\n", (uint32_t)(t_start >> 32), (uint32_t)t_start);
    initial_value = READ_GPIO();
    transmit_symbol();
    // wait until SS is over
    t_tmp = read_global_timer();
    while (t_tmp - t_start < (uint64_t) SS_LEN) {
      if ((initial_value == 1) && (hard_sync == 0) && (recessive_cntr > 5)) {
        new_value = READ_GPIO();
        if (new_value == 0) {
          t_start = t_tmp;
          hard_sync = 1;
          if (TILE_ID == 1 && DEBUG_CAN_PHY & DEBUG_CAN_PHY_CLK) xil_printf("%04u/%010u: HARD SYNC\n", (uint32_t)(t_tmp >> 32), (uint32_t)t_tmp);
        }
      }
      t_tmp = read_global_timer();
    }

    MEM->current_segment = PROP_SEG;
    if (TILE_ID == 1 && DEBUG_CAN_PHY & DEBUG_CAN_PHY_CLK) xil_printf("%04u/%010u: PROP_SEG\n", (uint32_t)(t_tmp >> 32), (uint32_t)t_tmp);
    initial_value = READ_GPIO();
    end_of_ss = t_tmp;

    // wait until Prop Seg is over
    while (t_tmp - t_start < (uint64_t)(SS_LEN + PROP_LEN)) {
      if ((initial_value == 1) && (hard_sync == 0) && (resync == 0)) {
        // we don't do both the hard sync and resync for one symbol
        // detect Recessive to Dominant edge
        new_value = READ_GPIO();
        if (new_value == 0) {
          ps1_extension = t_tmp - end_of_ss;
          resync = 1;
          if (TILE_ID == 1 && DEBUG_CAN_PHY & DEBUG_CAN_PHY_CLK) xil_printf("%04u/%010u: RESYNC (late edge)\n", (uint32_t)(t_tmp >> 32), (uint32_t)t_tmp);
        }
      }
      t_tmp = read_global_timer();
    }
    MEM->current_segment = PHASE_SEG_1;
    if (TILE_ID == 1 && DEBUG_CAN_PHY & DEBUG_CAN_PHY_CLK) xil_printf("%04u/%010u: PHASE_SEG_1\n", (uint32_t)(t_tmp >> 32), (uint32_t)t_tmp);
    // PHASE_SEG_1 : sample the bus
    while (read_global_timer() < t_start + (uint64_t)(SS_LEN + PROP_LEN + ps1_extension + PS_1_LEN/2));
    samples[0] = READ_GPIO();

    while (read_global_timer() < t_start + (uint64_t)(SS_LEN + PROP_LEN + ps1_extension + PS_1_LEN * 0.75));
    samples[1] = READ_GPIO();

    while (read_global_timer() < t_start + (uint64_t)(SS_LEN + PROP_LEN + ps1_extension + PS_1_LEN));
    samples[2] = READ_GPIO();
    resolved_value = (samples[0] + samples[1] + samples[2] > 1) ? RECESSIVE : DOMINANT;
    MEM->read_symbol = resolved_value;
    if (resolved_value == RECESSIVE)
      recessive_cntr ++;
    else
      recessive_cntr = 0;
    if (TILE_ID == 1 && DEBUG_CAN_PHY & DEBUG_CAN_PHY_RX) {
      xil_printf("%04u/%010u: resolved symbol on bus is %s %d\n",
          (uint32_t)(t_tmp >> 32), (uint32_t)t_tmp, (resolved_value == RECESSIVE ? "RECESSIVE" : "DOMINANT"), resolved_value);
    }
    if (TILE_ID == 1 && DEBUG_CAN_PHY & DEBUG_CAN_PHY_LED) {
      if (resolved_value == DOMINANT) set_led_color(0, 1, 0, 0);
      else set_led_color(0, 0, 1, 0);
    }

#ifdef GENERATE_VCD
    // send to ARM (time stamp first, then bus value)
    t_tmp = read_global_timer();
    for (int i = 0; i < 8; ++i) {
      on_riscv_snd_from_riscv_to_arm((uint8_t)(t_tmp >> (i * 8)));
    }
    on_riscv_snd_from_riscv_to_arm((uint8_t)resolved_value);
#endif

    // PHASE_SEG_2
    t_tmp = read_global_timer();
    MEM->current_segment = PHASE_SEG_2;
    if (TILE_ID == 1 && DEBUG_CAN_PHY & DEBUG_CAN_PHY_CLK) xil_printf("%04u/%010u: PHASE_SEG_2\n", (uint32_t)(t_tmp >> 32), (uint32_t)t_tmp);
    while (read_global_timer() - t_start < (uint64_t)(SS_LEN + PROP_LEN + PS_1_LEN + ps1_extension + PS_2_LEN)) {
      if ((initial_value == 1) && (hard_sync == 0) && (resync == 0)) {
        // detect Recessive to Dominant edge (early edge)
        new_value = READ_GPIO();
        if (new_value == 0) {
          resync = 1;
          if (TILE_ID == 1 && DEBUG_CAN_PHY & DEBUG_CAN_PHY_CLK) xil_printf("CAN PHY driver: resync (early edge) @ %04u/%010u\n", (uint32_t)(t_tmp >> 32), (uint32_t)t_tmp);
          break;
        }
      }
    }

    for (unsigned int i = 2; i < (2 + MAX_NUM_CAN_NODES); ++i) {
      MEM->node_states[i].has_read_during_this_symbol = 0;
      MEM->node_states[i].has_written_during_this_symbol = 0;
    }
    MEM->read_symbol = UNSPECIFIED;
    hard_sync = 0;
    resync = 0;
    ps1_extension = 0;
    ps2_shrinkage = 0;
    t_tmp = read_global_timer();
    if (TILE_ID == 1 && DEBUG_CAN_PHY & DEBUG_CAN_PHY_CLK) xil_printf("%04u/%010u: SYMBOL END\n", (uint32_t)(t_tmp >> 32), (uint32_t)t_tmp);
    if (TILE_ID == 1 && DEBUG_CAN_PHY & DEBUG_CAN_PHY_LED) set_led_color(0, 0, 0, 0);
  }
  return 0;
}


void transmit_symbol()
{
  uint64_t t_tmp = read_global_timer();
#ifdef GENERATE_VCD
  for (int i = 0; i < 8; ++i) {
    on_riscv_snd_from_riscv_to_arm((uint8_t)(t_tmp >> (i * 8)));
  }
#endif
  CAN_SYMBOL resolved = RECESSIVE;

  for (unsigned int i = 2; i < 2 + MAX_NUM_CAN_NODES; ++i) {
    if (MEM->transmitting_symbols[i] == DOMINANT) resolved = DOMINANT;
#ifdef GENERATE_VCD
    on_riscv_snd_from_riscv_to_arm((uint8_t)MEM->transmitting_symbols[i]);
#endif
    MEM->transmitting_symbols[i] = UNSPECIFIED;
  }
  gpio[GPIO_RW] = resolved;
  if (DEBUG_CAN_PHY & DEBUG_CAN_PHY_TX) {
    xil_printf("%04u/%010u: transmit symbol %s %d\n",
        (uint32_t)(t_tmp >> 32), (uint32_t)t_tmp, (resolved == RECESSIVE ? "RECESSIVE" : "DOMINANT"), resolved);
  }
}
