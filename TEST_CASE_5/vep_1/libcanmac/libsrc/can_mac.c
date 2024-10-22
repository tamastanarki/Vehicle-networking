#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <xil_printf.h>
#include <timers.h>
#include "can_phy.h"
#include "can_mac.h"
#include "settings.h"

#if(defined(TEST_CASE_5))
bool crc_zero_test = true;
struct CAN_CHECK_{

	uint32_t crc_tracker;
	bool crc_calc_check;
};
typedef struct CAN_CHECK_ CAN_CHECK;	
	
void can_state_initial(CAN_CHECK* current_state) {
	current_state->crc_calc_check = false;
	current_state->crc_tracker = 0;
}	


void calc_curr_crc_bit(CAN_SYMBOL data_sym, CAN_CHECK* current_state){
  
	current_state->crc_tracker = (current_state->crc_tracker << 1 ) | data_sym;
	if(current_state->crc_tracker & 0x4599){
		
		current_state->crc_tracker = current_state->crc_tracker^CRC_BASE;
		
		}
  //current_state->crc_tracker = current_state->crc_tracker & 0xFFFF;
 // xil_printf("CRC CALC CHECKER : %x \n NEW CRC TRACKER :%x \n", current_state->crc_calc_check, current_state->crc_tracker);  
}


#endif

#if(defined(TEST_CASE_1) || defined(TEST_CASE_2) || defined(TEST_CASE_3))
void send(CAN_SYMBOL data_sym)
{
  CAN_SYMBOL right_sym;

  can_phy_tx_symbol(data_sym);
  can_phy_rx_symbol(&right_sym);
}

#elif(defined(TEST_CASE_4))
CAN_SYMBOL send(CAN_SYMBOL data_sym)
{
  CAN_SYMBOL right_sym;

  can_phy_tx_symbol(data_sym);
  can_phy_rx_symbol(&right_sym);
  return right_sym;
}
#endif

#if(defined(TEST_CASE_5))
	CAN_SYMBOL send(CAN_SYMBOL data_sym, CAN_CHECK* current_state)
{
  CAN_SYMBOL right_sym;

  can_phy_tx_symbol(data_sym);
  can_phy_rx_symbol(&right_sym);
  
  if(current_state->crc_calc_check == true){
	  calc_curr_crc_bit(data_sym,current_state);
  
  }
  return right_sym;
}



#endif

#if(defined(TEST_CASE_1) || defined(TEST_CASE_2)  || defined(TEST_CASE_3) || defined(TEST_CASE_4))

void send_decon_uint32(uint32_t struct_data, int lenght){
    for (int i = lenght - 1 ; i >= 0; --i){
        CAN_SYMBOL sym_data = (struct_data >> i) & 1;
        
        send(sym_data);
        }
}

#endif

#if(defined(TEST_CASE_5))

void send_decon_uint32(uint32_t struct_data, int lenght, CAN_CHECK* current_state){
    for (int i = lenght - 1 ; i >= 0; --i){
        CAN_SYMBOL sym_data = (struct_data >> i) & 1;
        
        send(sym_data, current_state);
        }
}


#endif

#if(defined(TEST_CASE_1) || defined(TEST_CASE_2)  || defined(TEST_CASE_3) || defined(TEST_CASE_4))
void send_decon_uint64(uint64_t struct_data, int lenght){
    for (int i = lenght - 1 ; i >= 0; --i){
        CAN_SYMBOL sym_data = (struct_data >> i) & 1;
        
        send(sym_data);
        }
}
#endif



#if(defined(TEST_CASE_5))
void send_decon_uint64(uint64_t struct_data, int lenght, CAN_CHECK* current_state){
    for (int i = lenght - 1 ; i >= 0; --i){
        CAN_SYMBOL sym_data = (struct_data >> i) & 1;
        
        send(sym_data, current_state);
        }
}
#endif

#if(defined(TEST_CASE_1) || defined(TEST_CASE_2)  || defined(TEST_CASE_3) || defined(TEST_CASE_4))
void receive(CAN_SYMBOL* data_sym)
{
  can_phy_rx_symbol(data_sym);
  
}
#endif

#if(defined(TEST_CASE_5))
void receive(CAN_SYMBOL* data_sym, CAN_CHECK* current_state)
{
  can_phy_rx_symbol(data_sym);
  if(current_state->crc_calc_check == true){
	  calc_curr_crc_bit(*data_sym, current_state);
  }
}
#endif


#if(defined(TEST_CASE_1) || defined(TEST_CASE_2)  || defined(TEST_CASE_3) || defined(TEST_CASE_4))
void receive_decon_uint32(uint32_t* struct_data, int lenght)
{
	CAN_SYMBOL sym_data;

	for(int i = lenght - 1; i >= 0; i--)
  {
		receive(&sym_data);
    *struct_data |= (sym_data << i);
  }
}
#endif

#if(defined(TEST_CASE_5))
void receive_decon_uint32(uint32_t* struct_data, int lenght, CAN_CHECK* current_state)
{
	CAN_SYMBOL sym_data;

	for(int i = lenght - 1; i >= 0; i--)
  {
		receive(&sym_data,current_state);
    *struct_data |= (sym_data << i);
  }
}
#endif


#if(defined(TEST_CASE_1) || defined(TEST_CASE_2)  || defined(TEST_CASE_3) || defined(TEST_CASE_4))
void receive_decon_uint64(uint64_t* struct_data, int lenght)
{
	CAN_SYMBOL sym_data;

	for(int i = lenght - 1; i >= 0; i--)
  {
		receive(&sym_data);
    *struct_data |= (sym_data << i);
  }
}
#endif

#if(defined(TEST_CASE_5))
void receive_decon_uint64(uint64_t* struct_data, int lenght, CAN_CHECK* current_state)
{
	CAN_SYMBOL sym_data;

	for(int i = lenght - 1; i >= 0; i--)
  {
		receive(&sym_data, current_state);
    *struct_data |= (sym_data << i);
  }
}
#endif

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

#if(defined(TEST_CASE_1) || defined(TEST_CASE_2) || defined(TEST_CASE_3))
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

#elif(defined(TEST_CASE_4))
bool can_mac_tx_frame(CAN_FRAME* txFrame)
{
  bool success = true;
  send(DOMINANT);                           //SOF
  send_decon_uint32(txFrame->ID, 11);                 //Identifier
  send(DOMINANT);                           //RTR
  send(DOMINANT);                           //IDE
  send(DOMINANT);                           //r0
  send_decon_uint32(txFrame->DLC, 4);                 //DLC
  send_decon_uint64(txFrame->Data, txFrame->DLC * 8); //Data depending on DLC
  send_decon_uint32(txFrame->CRC, 15);                //CRC
  send(RECESSIVE);                          //CRC delimiter
  
  if(send(RECESSIVE) != DOMINANT)           //Acknowledge
  {
    success = false;
  }
  send(RECESSIVE);                          //Acknowledge delimiter
  send_decon_uint32(0xFFFF, 10);                      //EOF & interframe space filler
  return success;
}

#elif(defined(TEST_CASE_5))
bool can_mac_tx_frame(CAN_FRAME* txFrame)
{
  CAN_CHECK* current_state = (CAN_CHECK*)malloc(sizeof(CAN_CHECK));
  bool success = true;
  can_state_initial(current_state);
  current_state->crc_calc_check = true;
  send(DOMINANT, current_state);                           //SOF
  send_decon_uint32(txFrame->ID, 11, current_state);                 //Identifier
  send(DOMINANT, current_state);                           //RTR
  send(DOMINANT, current_state);                           //IDE
  send(DOMINANT, current_state);                           //r0
  send_decon_uint32(txFrame->DLC, 4, current_state);                 //DLC
  send_decon_uint64(txFrame->Data, txFrame->DLC * 8, current_state); //Data depending on DLC
  if (crc_zero_test == true){
  current_state->crc_calc_check = false;
  send_decon_uint32(0, 15, current_state);
  }
  else if(((txFrame->CRC >> 31) & 1 ) == 1){
    current_state->crc_calc_check = false;
	  send_decon_uint32(txFrame->CRC,15, current_state); //crc 
	  }
    
  else
  {
	for(int i = 0 ; i < 15 ; ++i){
	 //calc_curr_crc_bit(DOMINANT, current_state);
	}
  //xil_printf("CRC before decon %x \n :", current_state->crc_tracker) ;
  current_state->crc_calc_check = false;
	send_decon_uint32(current_state->crc_tracker,15, current_state);
	  //xil_printf("CRC After decon: %x \n", current_state->crc_tracker) ;
  }
  send(RECESSIVE, current_state);                          //CRC delimiter
  
  if(send(RECESSIVE, current_state) != DOMINANT)           //Acknowledge
  {
    success = false;
  }
  send(RECESSIVE, current_state);                          //Acknowledge delimiter
  send_decon_uint32(0xFFFF, 10, current_state);                      //EOF & interframe space filler
  crc_zero_test = false;
  free(current_state);
  return success;
}
#endif










#if(defined(TEST_CASE_1) || defined(TEST_CASE_2) || defined(TEST_CASE_3))
void can_mac_rx_frame(CAN_FRAME* rxFrame)
{
  CAN_SYMBOL sym_data;
  can_state_initial(current_state);
  uint32_t eof;

  //First dominant bit, indicates the SOF
  wait_for_dominant();                          //SOF

	receive_decon_uint32(&(rxFrame->ID), 11);                 //Identifier
	receive(&sym_data);                                  //RTR
  receive(&sym_data);                                  //IDE
  receive(&sym_data);                                  //r0
	receive_decon_uint32(&(rxFrame->DLC), 4);                 //DLC
  receive_decon_uint64(&(rxFrame->Data), rxFrame->DLC * 8); //Data depending on DLC
  //calc CRC 
  
  receive_decon_uint32(&(rxFrame->CRC), 15);                //CRC
  receive(&sym_data);                                  //CRC delimiter
  receive(&sym_data);                                  //Acknowledge
  receive(&sym_data);                                  //Acknowledge delimiter
  receive_decon_uint32(&eof, 7);                            //EOF & interframe space filler
}

#elif(defined(TEST_CASE_4))
void can_mac_rx_frame(CAN_FRAME* rxFrame)
{
  CAN_SYMBOL sym_data;
  uint32_t eof;

  //First dominant bit, indicates the SOF
  wait_for_dominant();                          //SOF

	receive_decon_uint32(&(rxFrame->ID), 11);                 //Identifier
	receive(&sym_data);                                       //RTR
  receive(&sym_data);                                       //IDE
  receive(&sym_data);                                       //r0
	receive_decon_uint32(&(rxFrame->DLC), 4);                 //DLC
  receive_decon_uint64(&(rxFrame->Data), rxFrame->DLC * 8); //Data depending on DLC
  receive_decon_uint32(&(rxFrame->CRC), 15);                //CRC
  receive(&sym_data);                                       //CRC delimiter
#ifdef ACK_CHECK                                    //Acknowledgement fail test ca
  send(DOMINANT);                  
#else
  receive(&sym_data);                                       //Acknowledge
#endif
  receive(&sym_data);                                       //Acknowledge delimiter
  receive_decon_uint32(&eof, 7);                            //EOF & interframe space filler
}


#elif(defined(TEST_CASE_5))
void can_mac_rx_frame(CAN_FRAME* rxFrame)
{
  bool tester = false;
  bool crc_check = true;
  while(!tester)
  {
  
  
  CAN_SYMBOL sym_data;
  uint32_t eof;

  wait_for_dominant();                          //SOF
  CAN_CHECK* current_state = (CAN_CHECK*)malloc(sizeof(CAN_CHECK));
  can_state_initial(current_state);
  current_state->crc_calc_check = true;
  
  receive_decon_uint32(&(rxFrame->ID), 11, current_state);                 //Identifier
  receive(DOMINANT, current_state);                           //RTR
  receive(DOMINANT, current_state);                           //IDE
  receive(DOMINANT, current_state);                           //r0
  receive_decon_uint32(&(rxFrame->DLC), 4, current_state);                 //DLC
  
  receive_decon_uint64(&(rxFrame->Data), rxFrame->DLC * 8, current_state); //Data depending on DLC
  current_state->crc_calc_check = false; 
  receive_decon_uint32(&(rxFrame->CRC),15, current_state); //crc
  
  xil_printf("Comparing CRC From reciver 0x%x and second variable CRC from sender 0x%x\n" ,(current_state->crc_tracker&0x7FFF), (rxFrame->CRC&0x7FFF));  
  receive(&sym_data, current_state);                                      //CRC delimiter

  if ((current_state->crc_tracker&0x7FFF) != (rxFrame->CRC&0x7FFF)){
   crc_check = false;
   current_state->crc_calc_check = false;
  }
  
  if(crc_check == false){
	 xil_printf("CRC FAIL, Message id 0x%x, DLC 0x%x, Data: 0x%08x%08x, CRC: 0x%x FAIL .\n",rxFrame->ID , rxFrame->DLC,(uint32_t)(rxFrame->Data >>32),(uint32_t)rxFrame->Data, rxFrame->CRC);
	 crc_check = true;
	receive(&sym_data, current_state); // aCK
	crc_zero_test = false;
	  
	  
	 //set variables back
	 
	 rxFrame->ID = 0;
	 rxFrame->DLC = 0;
	 rxFrame->Data = 0;
	 rxFrame->CRC = 0;
  }
  else
  {
	  send(DOMINANT,current_state);
    tester = true;
	  
  }
  //current_state->crc_calc_check = true;
	receive(&sym_data, current_state);
	receive_decon_uint32(&eof, 7,current_state);
	
  
 }
}
#endif
























void wait(uint64_t time)
{
  while(read_global_timer() < time)
  {
    
  }
}
