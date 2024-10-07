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
