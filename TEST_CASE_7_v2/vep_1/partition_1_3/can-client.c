#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <timers.h>
#include <xil_printf.h>
#include "can_phy.h"
#include "can_phy.c"
#include "can_mac.h"
#include "settings.h"
#include "io.h"
#include "io.c"

int main (void)
{
#if defined(TEST_CASE_1)
  uint64_t t = read_global_timer();
  xil_printf("%04u/%010u: CAN client %d %d running in TEST_CASE_1\n", (uint32_t)(t >> 32), (uint32_t)t, TILE_ID, PARTITION_ID);
  while(true)
  {
    CAN_FRAME* frame = (CAN_FRAME*)malloc(sizeof(CAN_FRAME));
    frame->ID = 0;
    frame->DLC = 0;
    frame->Data = 0;
    frame->CRC = 0;
    can_mac_rx_frame(frame);
    xil_printf("CAN client %d %d Frame received id: 0x%x, DLC: 0x%x, Data: 0x%08x%08x, CRC: 0x%x\n",TILE_ID, PARTITION_ID, frame->ID, frame->DLC, (uint32_t)(frame->Data >> 32), (uint32_t)frame->Data, frame->CRC);
    if(frame->ID == 0 && frame->DLC == 1 && frame->Data == 1 && frame->CRC == 0)
    {
      turn_led_on(0);
      wait(read_global_timer() +  SYMBOL_LENGTH_CYCLES);
      turn_led_off(0);
    }
    free(frame);
  }
#elif defined(TEST_CASE_2)
  uint64_t t = read_global_timer();
  xil_printf("%04u/%010u: CAN client %d %d running in TEST_CASE_2\n", (uint32_t)(t >> 32), (uint32_t)t, TILE_ID, PARTITION_ID);
  while(true)
  {
    CAN_FRAME* frame = (CAN_FRAME*)malloc(sizeof(CAN_FRAME));
    frame->ID = 0;
    frame->DLC = 0;
    frame->Data = 0;
    frame->CRC = 0;
    can_mac_rx_frame(frame);
    xil_printf("CAN client %d %d Frame received id: 0x%x, DLC: 0x%x, Data: 0x%08x%08x, CRC: 0x%x\n",TILE_ID, PARTITION_ID, frame->ID, frame->DLC, (uint32_t)(frame->Data >> 32), (uint32_t)frame->Data, frame->CRC);
    free(frame);
  }
#elif defined(TEST_CASE_3)
  uint64_t t = read_global_timer();
  xil_printf("%04u/%010u: CAN client %d %d running in TEST_CASE_3\n", (uint32_t)(t >> 32), (uint32_t)t, TILE_ID, PARTITION_ID);
  while(true)
  {
    CAN_FRAME* frame = (CAN_FRAME*)malloc(sizeof(CAN_FRAME));
    frame->ID = 0;
    frame->DLC = 0;
    frame->Data = 0;
    frame->CRC = 0;
    can_mac_rx_frame(frame);
    xil_printf("CAN client %d %d Frame received id: 0x%x, DLC: 0x%x, Data: 0x%08x%08x, CRC: 0x%x\n",TILE_ID, PARTITION_ID, frame->ID, frame->DLC, (uint32_t)(frame->Data >> 32), (uint32_t)frame->Data, frame->CRC);
    free(frame);
  }
#elif defined(TEST_CASE_4) || defined(TEST_CASE_5)
  uint64_t t = read_global_timer();
  xil_printf("%04u/%010u: CAN client %d %d running in TEST_CASE_4 or 5\n", (uint32_t)(t >> 32), (uint32_t)t, TILE_ID, PARTITION_ID);
  while(true)
  {
    CAN_FRAME* frame = (CAN_FRAME*)malloc(sizeof(CAN_FRAME));
    frame->ID = 0;
    frame->DLC = 0;
    frame->Data = 0;
    frame->CRC = 0;
    can_mac_rx_frame(frame);
    xil_printf("CAN client %d %d Frame received id: 0x%x, DLC: 0x%x, Data: 0x%08x%08x, CRC: 0x%x\n",TILE_ID, PARTITION_ID, frame->ID, frame->DLC, (uint32_t)(frame->Data >> 32), (uint32_t)frame->Data, frame->CRC);
    free(frame);
  }
#elif (defined(TEST_CASE_6) || defined(TEST_CASE_7))
  //case_6_sender(PRIO_PERIOD0, 0x1, 0, 0, TILE_ID, PARTITION_ID);
  uint64_t t = read_global_timer();
  xil_printf("%04u/%010u: CAN client %d %d running in TEST_CASE_6\n", (uint32_t)(t >> 32), (uint32_t)t, TILE_ID, PARTITION_ID);
  CAN_FRAME* frame = (CAN_FRAME*)malloc(sizeof(CAN_FRAME));
  frame->ID = PRIO_PERIOD0;
  frame->DLC = 0x1;
  frame->Data = 0;
  frame->CRC = 0;
  while(true)
  {
    uint64_t startTime = read_global_timer();
    uint64_t deadline = startTime + PERIOD0;
    
    xil_printf("CAN client %d %d Frame sending id: 0x%x, DLC: 0x%x, Data: 0x%08x%08x, CRC: 0x%x\n",TILE_ID, PARTITION_ID, frame->ID, frame->DLC, (uint32_t)(frame->Data >> 32), (uint32_t)frame->Data, frame->CRC);
    
    while(!can_mac_tx_frame(frame))
    {
      if(read_global_timer() > deadline - (FRAME_LENGTH_CYCLES + SYMBOL_LENGTH_CYCLES * (25 + 8*frame->DLC + 44 + ((34 + 8*frame->DLC - 1) / 4))))
      {
        xil_printf("Failed, no time to resend\n");
        break;
      }
      xil_printf("Failed, resending\n");
    }
    wait(deadline);
  }
  free(frame);
#else
  /******* CAN Client is disabled in this test case *******/
  uint64_t t = read_global_timer();
  xil_printf("%04u/%010u: CAN client %d %d is idle\n", (uint32_t)(t >> 32), (uint32_t)t, TILE_ID, PARTITION_ID);
  while (1) { asm("wfi"); }
#endif
  return 0;
}
