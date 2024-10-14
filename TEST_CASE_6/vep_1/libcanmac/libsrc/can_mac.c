#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <xil_printf.h>
#include <timers.h>
#include "can_phy.h"
#include "can_mac.h"

void send(CAN_SYMBOL data_sym)
{
  CAN_SYMBOL right_sym;

  can_phy_tx_symbol(data_sym);
  can_phy_rx_symbol(&right_sym);
}

void send_decon_uint32(uint32_t struct_data, int lenght){
    for (int i = lenght - 1 ; i >= 0; --i){
        CAN_SYMBOL sym_data = (struct_data >> i) & 1;
        
        send(sym_data);
        }
}

void send_decon_uint64(uint64_t struct_data, int lenght){
    for (int i = lenght - 1 ; i >= 0; --i){
        CAN_SYMBOL sym_data = (struct_data >> i) & 1;
        
        send(sym_data);
        }
}

void receive(CAN_SYMBOL* data_sym)
{
  can_phy_rx_symbol(data_sym);
}

void receive_decon_uint32(uint32_t* struct_data, int lenght)
{
	CAN_SYMBOL sym_data;

	for(int i = lenght - 1; i >= 0; i--)
  {
		receive(&sym_data);
    *struct_data |= (sym_data << i);
  }
}

void receive_decon_uint64(uint64_t* struct_data, int lenght)
{
	CAN_SYMBOL sym_data;

	for(int i = lenght - 1; i >= 0; i--)
  {
		receive(&sym_data);
    *struct_data |= (sym_data << i);
  }
}

void wait_for_dominant()
{
  CAN_SYMBOL sym_data;

  while(true)
  {
    can_phy_rx_symbol(&sym_data);
    if(sym_data == DOMINANT)
    {
      break;
    }
  }
}

bool can_mac_tx_frame(CAN_FRAME* txFrame)
{
  send(DOMINANT);                                 //SOF
  send_decon_uint32(txFrame->ID, 11);                 //Identifier
  send(DOMINANT);                                 //RTR
  send(DOMINANT);                                 //IDE
  send(DOMINANT);                                 //r0
  send_decon_uint32(txFrame->DLC, 4);                 //DLC
  send_decon_uint64(txFrame->Data, txFrame->DLC * 8); //Data depending on DLC
  send_decon_uint32(txFrame->CRC, 15);                //CRC
  send(RECESSIVE);                                //CRC delimiter
  send(RECESSIVE);                                //Acknowledge
  send(RECESSIVE);                                //Acknowledge delimiter
  send_decon_uint32(0xFFFF, 10);                      //EOF & interframe space filler
  return true;
}

void can_mac_rx_frame(CAN_FRAME* rxFrame)
{
  CAN_SYMBOL sym_data;
  uint32_t eof;

  //First dominant bit, indicates the SOF
  wait_for_dominant();                          //SOF

	receive_decon_uint32(&(rxFrame->ID), 11);                 //Identifier
	receive(&sym_data);                                  //RTR
  receive(&sym_data);                                  //IDE
  receive(&sym_data);                                  //r0
	receive_decon_uint32(&(rxFrame->DLC), 4);                 //DLC
  receive_decon_uint64(&(rxFrame->Data), rxFrame->DLC * 8); //Data depending on DLC
  receive_decon_uint32(&(rxFrame->CRC), 15);                //CRC
  receive(&sym_data);                                  //CRC delimiter
  receive(&sym_data);                                  //Acknowledge
  receive(&sym_data);                                  //Acknowledge delimiter
  receive_decon_uint32(&eof, 7);                            //EOF & interframe space filler
}

void wait(uint64_t time)
{
  while(read_global_timer() < time)
  {
    
  }
}

void case_6_sender(uint32_t ID, uint32_t DLC, uint32_t Data, uint32_t CRC)
{
  uint64_t t = read_global_timer();
  xil_printf("%04u/%010u: CAN client %d %d running in TEST_CASE_6\n", (uint32_t)(t >> 32), (uint32_t)t, TILE_ID, PARTITION_ID);
  /******* insert CAN Client code for TEST_CASE_6 here *******/
  CAN_FRAME* frame = (CAN_FRAME*)malloc(sizeof(CAN_FRAME));
  frame->ID = ID;
  frame->DLC = DLC;
  frame->Data = Data;
  frame->CRC = CRC;
  while(true)
  {
    uint64_t startTime = read_global_timer();
    uint64_t deadline = startTime + ID;
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
    xil_printf("CAN client %d %d Frame sending id: 0x%x, DLC: 0x%x, Data: 0x%08x%08x, CRC: 0x%x\n",TILE_ID, PARTITION_ID, frame->ID, frame->DLC, (uint32_t)(frame->Data >> 32), (uint32_t)frame->Data, frame->CRC);
  }
  free(frame);
}

void case_6_receiver(uint32_t Period)
{
  uint64_t t = read_global_timer();
  xil_printf("%04u/%010u: CAN client %d %d running in TEST_CASE_6\n", (uint32_t)(t >> 32), (uint32_t)t, TILE_ID, PARTITION_ID);
  /******* insert CAN Client code for TEST_CASE_6 here *******/
  while(true)
  {
    CAN_FRAME* frame = (CAN_FRAME*)malloc(sizeof(CAN_FRAME));
    frame->ID = 0;
    frame->DLC = 0;
    frame->Data = 0;
    frame->CRC = 0;
    can_mac_rx_frame(frame);
    if (frame->ID != Period) {
      free(frame);
      continue;
    }
    xil_printf("CAN client %d %d Receiving frame id: 0x%x, DLC: 0x%x, Data: 0x%08x%08x, CRC: 0x%x\n", TILE_ID, PARTITION_ID, frame->ID, frame->DLC, (uint32_t)(frame->Data >> 32), (uint32_t)frame->Data, frame->CRC);
    free(frame);
  }
}
