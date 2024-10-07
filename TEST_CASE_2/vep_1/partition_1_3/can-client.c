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
#if (defined(TEST_CASE_1))
  uint64_t t = read_global_timer();
  xil_printf("%04u/%010u: CAN client %d %d running in TEST_CASE_1\n", (uint32_t)(t >> 32), (uint32_t)t, TILE_ID, PARTITION_ID);
  /******* insert CAN Client code for TEST_CASE_3 here *******/
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
      //xil_printf("turning led 1 on");
    }
    
    free(frame);
  }


#elif (defined(TEST_CASE_2))
  uint64_t t = read_global_timer();
  xil_printf("%04u/%010u: CAN client %d %d running in TEST_CASE_2\n", (uint32_t)(t >> 32), (uint32_t)t, TILE_ID, PARTITION_ID);
  /******* insert CAN Client code for TEST_CASE_3 here *******/
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















#else
  /******* CAN Client is disabled in this test case *******/
  uint64_t t = read_global_timer();
  xil_printf("%04u/%010u: CAN client %d %d is idle\n", (uint32_t)(t >> 32), (uint32_t)t, TILE_ID, PARTITION_ID);
  while (1) { asm("wfi"); }
#endif
  return 0;
}
