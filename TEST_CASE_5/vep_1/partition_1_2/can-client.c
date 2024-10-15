#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
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
  #if (defined(TEST_CASE_1))
  xil_printf("%04u/%010u: CAN client %d %d running in TEST_CASE_1\n", (uint32_t)(t >> 32), (uint32_t)t, TILE_ID, PARTITION_ID);
  #elif (defined(TEST_CASE_2))
  xil_printf("%04u/%010u: CAN client %d %d running in TEST_CASE_2\n", (uint32_t)(t >> 32), (uint32_t)t, TILE_ID, PARTITION_ID);
  #endif
  /******* insert CAN Client code for TEST_CASE_1 and 2 here *******/


  CAN_FRAME* frame = (CAN_FRAME*)malloc(sizeof(CAN_FRAME));
  frame->ID = 0;
  frame->DLC = 1;
  frame->Data = 1;
  frame->CRC = 0;

  while(true)
  {
    uint8_t button0 = read_button_state(0);
    if(!button0)
    {
      continue;
    }
    xil_printf("CAN client %d %d Sending frame id: 0x%x, DLC: 0x%x, Data: 0x%08x%08x, CRC: 0x%x\n", TILE_ID, PARTITION_ID, frame->ID, frame->DLC, (uint32_t)(frame->Data >> 32), (uint32_t)frame->Data, frame->CRC);
    can_mac_tx_frame(frame);
  }
  
  free(frame);
  
  
#elif defined(TEST_CASE_2)

 uint64_t t = read_global_timer();
 
  xil_printf("%04u/%010u: CAN client %d %d running in TEST_CASE_2\n", (uint32_t)(t >> 32), (uint32_t)t, TILE_ID, PARTITION_ID);

  CAN_FRAME* frame = (CAN_FRAME*)malloc(sizeof(CAN_FRAME));
  frame->ID = 0;
  frame->DLC = 3;
  frame->Data = 7;
  frame->CRC = 0;

  while(true)
  {
    uint64_t framesendend = read_global_timer()+PERIOD0;
    xil_printf("CAN client %d %d Sending frame id: 0x%x, DLC: 0x%x, Data: 0x%08x%08x, CRC: 0x%x\n", TILE_ID, PARTITION_ID, frame->ID, frame->DLC, (uint32_t)(frame->Data >> 32), (uint32_t)frame->Data, frame->CRC);
    xil_printf("timer %d \n",framesendend);
	can_mac_tx_frame(frame);
	//wait(framesendend);
  }
  free(frame);


#elif (defined(TEST_CASE_3))
  uint64_t t = read_global_timer();
  xil_printf("%04u/%010u: CAN client %d %d running in TEST_CASE_3\n", (uint32_t)(t >> 32), (uint32_t)t, TILE_ID, PARTITION_ID);
  /******* insert CAN Client code for TEST_CASE_3 here *******/

  while(true)
  {
    CAN_FRAME* frame_1 = (CAN_FRAME*)malloc(sizeof(CAN_FRAME));
    frame_1->ID = 0;
    frame_1->DLC = 7;
    frame_1->Data = 3;
    frame_1->CRC = 0;
    uint64_t framesendend = read_global_timer()+PERIOD0;
    xil_printf("CAN client %d %d Sending frame id: 0x%x, DLC: 0x%x, Data: 0x%08x%08x, CRC: 0x%x\n", TILE_ID, PARTITION_ID, frame_1->ID, frame_1->DLC, (uint32_t)(frame_1->Data >> 32), (uint32_t)frame_1->Data, frame_1->CRC);
    xil_printf("timer %d \n",framesendend);
	can_mac_tx_frame(frame_1);
	//wait(framesendend);



    CAN_FRAME* frame_2 = (CAN_FRAME*)malloc(sizeof(CAN_FRAME));
    frame_2->ID = 0;
    frame_2->DLC = 3;
    frame_2->Data = 3;
    frame_2->CRC = 0;
    uint64_t framesendend = read_global_timer()+PERIOD0;
    xil_printf("CAN client %d %d Sending frame id: 0x%x, DLC: 0x%x, Data: 0x%08x%08x, CRC: 0x%x\n", TILE_ID, PARTITION_ID, frame_2->ID, frame_2->DLC, (uint32_t)(frame_2->Data >> 32), (uint32_t)frame_2->Data, frame_2->CRC);
    xil_printf("timer %d \n",framesendend);
	can_mac_tx_frame(frame_2);
	//wait(framesendend);



    CAN_FRAME* frame_3 = (CAN_FRAME*)malloc(sizeof(CAN_FRAME));
    frame_3->ID = 0;
    frame_3->DLC = 5;
    frame_3->Data = 3;
    frame_3->CRC = 0;
    uint64_t framesendend = read_global_timer()+PERIOD0;
    xil_printf("CAN client %d %d Sending frame id: 0x%x, DLC: 0x%x, Data: 0x%08x%08x, CRC: 0x%x\n", TILE_ID, PARTITION_ID, frame_3->ID, frame_3->DLC, (uint32_t)(frame_3->Data >> 32), (uint32_t)frame_3->Data, frame_3->CRC);
    xil_printf("timer %d \n",framesendend);
	can_mac_tx_frame(frame_3);
	//wait(framesendend);
  }
  free(frame_1);
  free(frame_2);
  free(frame_3);



#elif defined(TEST_CASE_4)

 uint64_t t = read_global_timer();
 
  xil_printf("%04u/%010u: CAN client %d %d running in TEST_CASE_4\n", (uint32_t)(t >> 32), (uint32_t)t, TILE_ID, PARTITION_ID);

  CAN_FRAME* frame = (CAN_FRAME*)malloc(sizeof(CAN_FRAME));
  frame->ID = 0;
  frame->DLC = 7;
  frame->Data = 3;
  frame->CRC = 0;

  while(true)
  {
    uint64_t framesendend = read_global_timer()+PERIOD0;
	while(!can_mac_tx_frame(frame))
  {
    if(read_global_timer() > framesendend - (FRAME_LENGTH_CYCLES + SYMBOL_LENGTH_CYCLES+FRAME_LENGTH_CYCLES * (25 + 8*frame->DLC + 44 + ((34 + 8*frame->DLC -1)/4))))
    {
      xil_printf("Did not acknowladge in time, cannot resend!\n");
      break;
    }
    xil_printf("Acknoeladge failed , Resending!\n");
  }
  wait(framesendend);
    xil_printf("CAN client %d %d Sending frame id: 0x%x, DLC: 0x%x, Data: 0x%08x%08x, CRC: 0x%x\n", TILE_ID, PARTITION_ID, frame->ID, frame->DLC, (uint32_t)(frame->Data >> 32), (uint32_t)frame->Data, frame->CRC);
    xil_printf("timer %d \n",framesendend);
  }
  free(frame);





#elif defined(TEST_CASE_5)

 uint64_t t = read_global_timer();
 
  xil_printf("%04u/%010u: CAN client %d %d running in TEST_CASE_4\n", (uint32_t)(t >> 32), (uint32_t)t, TILE_ID, PARTITION_ID);

  CAN_FRAME* frame = (CAN_FRAME*)malloc(sizeof(CAN_FRAME));
  frame->ID = 69;
  frame->DLC = 1;
  frame->Data = 777777;
  frame->CRC = 420;

  while(true)
  {
    uint64_t framesendend = read_global_timer()+PERIOD0;
	while(!can_mac_tx_frame(frame))
  {
    if(read_global_timer() > framesendend - (FRAME_LENGTH_CYCLES + SYMBOL_LENGTH_CYCLES+FRAME_LENGTH_CYCLES * (25 + 8*frame->DLC + 44 + ((34 + 8*frame->DLC -1)/4))))
    {
      xil_printf("Did not acknowladge in time, cannot resend!\n");
      break;
    }
    xil_printf("Acknoeladge failed , Resending!\n");
	frame->CRC = 3333;
  }
  wait(framesendend);
    xil_printf("CAN client %d %d Sending frame id: 0x%x, DLC: 0x%x, Data: 0x%08x%08x, CRC: 0x%x\n", TILE_ID, PARTITION_ID, frame->ID, frame->DLC, (uint32_t)(frame->Data >> 32), (uint32_t)frame->Data, frame->CRC);
    xil_printf("timer %d \n",framesendend);
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
