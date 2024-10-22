#include <stdint.h>
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
#if defined(TEST_CASE_6)
  //case_6_sender(PRIO_PERIOD1, 0x1, 0, 0, TILE_ID, PARTITION_ID);
  uint64_t t = read_global_timer();
  xil_printf("%04u/%010u: CAN client %d %d running in TEST_CASE_6\n", (uint32_t)(t >> 32), (uint32_t)t, TILE_ID, PARTITION_ID);
  CAN_FRAME* frame = (CAN_FRAME*)malloc(sizeof(CAN_FRAME));
  frame->ID = PRIO_PERIOD1;
  frame->DLC = 0x1;
  frame->Data = 0;
  frame->CRC = 0;
  while(true)
  {
    uint64_t startTime = read_global_timer();
    uint64_t deadline = startTime + PERIOD1;
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
