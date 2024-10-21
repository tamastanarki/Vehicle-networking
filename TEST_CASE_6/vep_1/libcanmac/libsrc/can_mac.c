#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <xil_printf.h>
#include <timers.h>
#include "can_phy.h"
#include "can_mac.h"
#include "settings.h"

#if defined(TEST_CASE_5)
  struct CAN_CHECK_
  {
    uint32_t crc_tracker;
    bool crc_calc_check;
  };
  typedef struct CAN_CHECK_ CAN_CHECK;	
  void can_state_initial(CAN_CHECK* current_state)
  {
    current_state->crc_calc_check = false;
    current_state->crc_tracker = 0;
  }
#elif defined(TEST_CASE_6)
  struct CAN_CHECK_
  {
    uint32_t crc_tracker;
    bool crc_calc_check;
    bool arbitration_check;
    bool arbitration_error;
  };
  typedef struct CAN_CHECK_ CAN_CHECK;	
  void can_state_initial(CAN_CHECK* current_state)
  {
    current_state->crc_calc_check = false;
    current_state->crc_tracker = 0;
    current_state->arbitration_check = false;
    current_state->arbitration_error = false;
  }
#endif
#if defined(TEST_CASE_5) || defined(TEST_CASE_6)
  void calc_curr_crc_bit(CAN_SYMBOL data_sym, CAN_CHECK* current_state)
  {
    current_state->crc_tracker = (current_state->crc_tracker << 1 ) | data_sym;
    if(current_state->crc_tracker & 0x8005)
    {
      current_state->crc_tracker = current_state->crc_tracker^CRC_BASE;
    }
  }
#endif
#if defined(TEST_CASE_1) || defined(TEST_CASE_2) || defined(TEST_CASE_3)
  void send(CAN_SYMBOL data_sym)
  {
    CAN_SYMBOL right_sym;
    can_phy_tx_symbol(data_sym);
    can_phy_rx_symbol(&right_sym);
  }
#elif defined(TEST_CASE_4)
  CAN_SYMBOL send(CAN_SYMBOL data_sym)
  {
    CAN_SYMBOL right_sym;
    can_phy_tx_symbol(data_sym);
    can_phy_rx_symbol(&right_sym);
    return right_sym;
  }
#elif defined(TEST_CASE_5)
	CAN_SYMBOL send(CAN_SYMBOL data_sym, CAN_CHECK* current_state)
  {
    CAN_SYMBOL right_sym;
    can_phy_tx_symbol(data_sym);
    can_phy_rx_symbol(&right_sym);
    if(current_state->crc_calc_check)
    {
      calc_curr_crc_bit(data_sym,current_state);
    }
    return right_sym;
  }
#elif defined(TEST_CASE_6)
	CAN_SYMBOL send(CAN_SYMBOL data_sym, CAN_CHECK* current_state)
  {
    CAN_SYMBOL right_sym;
    can_phy_tx_symbol(data_sym);
    can_phy_rx_symbol(&right_sym);
    if(current_state->crc_calc_check)
    {
      calc_curr_crc_bit(data_sym,current_state);
    }
    if(current_state->arbitration_check)
    {
      if(data_sym != right_sym)
      {
        current_state->arbitration_error = true;
      }
    }
    return right_sym;
  }
#endif
#if defined(TEST_CASE_1) || defined(TEST_CASE_2) || defined(TEST_CASE_3) || defined(TEST_CASE_4)
  void send_decon_uint32(uint32_t struct_data, int lenght)
  {
    for (int i = lenght - 1 ; i >= 0; --i)
    {
      CAN_SYMBOL sym_data = (struct_data >> i) & 1;
      send(sym_data);
    }
  }
#elif defined(TEST_CASE_5)
  void send_decon_uint32(uint32_t struct_data, int lenght, CAN_CHECK* current_state)
  {
    for (int i = lenght - 1 ; i >= 0; --i)
    {
      CAN_SYMBOL sym_data = (struct_data >> i) & 1;
      send(sym_data, current_state);
    }
  }
#elif defined(TEST_CASE_6)
  void send_decon_uint32(uint32_t struct_data, int lenght, CAN_CHECK* current_state)
  {
    for (int i = lenght - 1 ; i >= 0; --i)
    {
      CAN_SYMBOL sym_data = (struct_data >> i) & 1;
      send(sym_data, current_state);
      if(current_state->arbitration_check && current_state->arbitration_error)
      {
        return;
      }
    }
  }
#endif
#if defined(TEST_CASE_1) || defined(TEST_CASE_2) || defined(TEST_CASE_3) || defined(TEST_CASE_4)
  void send_decon_uint64(uint64_t struct_data, int lenght)
  {
    for (int i = lenght - 1 ; i >= 0; --i)
    {
      CAN_SYMBOL sym_data = (struct_data >> i) & 1;
      send(sym_data);
    }
  }
#elif defined(TEST_CASE_5)
  void send_decon_uint64(uint64_t struct_data, int lenght, CAN_CHECK* current_state)
  {
    for (int i = lenght - 1 ; i >= 0; --i)
    {
      CAN_SYMBOL sym_data = (struct_data >> i) & 1;
      send(sym_data, current_state);
    }
  }
#elif defined(TEST_CASE_6)
  void send_decon_uint64(uint64_t struct_data, int lenght, CAN_CHECK* current_state)
  {
    for (int i = lenght - 1 ; i >= 0; --i)
    {
      CAN_SYMBOL sym_data = (struct_data >> i) & 1;
      send(sym_data, current_state);
      if(current_state->arbitration_check && current_state->arbitration_error)
      {
        return;
      }
    }
  }
#endif
#if defined(TEST_CASE_1) || defined(TEST_CASE_2) || defined(TEST_CASE_3) || defined(TEST_CASE_4)
  void receive(CAN_SYMBOL* data_sym)
  {
    can_phy_rx_symbol(data_sym);
  }
#elif defined(TEST_CASE_5) || defined(TEST_CASE_6)
  void receive(CAN_SYMBOL* data_sym, CAN_CHECK* current_state)
  {
    can_phy_rx_symbol(data_sym);
    if(current_state->crc_calc_check)
    {
      calc_curr_crc_bit(*data_sym, current_state);
    }
  }
#endif
#if defined(TEST_CASE_1) || defined(TEST_CASE_2) || defined(TEST_CASE_3) || defined(TEST_CASE_4)
  void receive_decon_uint32(uint32_t* struct_data, int lenght)
  {
    CAN_SYMBOL sym_data;
    for(int i = lenght - 1; i >= 0; i--)
    {
      receive(&sym_data);
      *struct_data |= (sym_data << i);
    }
  }
#elif defined(TEST_CASE_5) || defined(TEST_CASE_6)
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
#if defined(TEST_CASE_1) || defined(TEST_CASE_2) || defined(TEST_CASE_3) || defined(TEST_CASE_4)
  void receive_decon_uint64(uint64_t* struct_data, int lenght)
  {
    CAN_SYMBOL sym_data;
    for(int i = lenght - 1; i >= 0; i--)
    {
      receive(&sym_data);
      *struct_data |= (sym_data << i);
    }
  }
#elif defined(TEST_CASE_5) || defined(TEST_CASE_6)
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
#if defined(TEST_CASE_1) || defined(TEST_CASE_2) || defined(TEST_CASE_3)
  bool can_mac_tx_frame(CAN_FRAME* txFrame)
  {
    send(DOMINANT);                                     //SOF
    send_decon_uint32(txFrame->ID, 11);                 //Identifier
    send(DOMINANT);                                     //RTR
    send(DOMINANT);                                     //IDE
    send(DOMINANT);                                     //r0
    send_decon_uint32(txFrame->DLC, 4);                 //DLC
    send_decon_uint64(txFrame->Data, txFrame->DLC * 8); //Data depending on DLC
    send_decon_uint32(txFrame->CRC, 15);                //CRC
    send(RECESSIVE);                                    //CRC delimiter
    send(RECESSIVE);                                    //Acknowledge
    send(RECESSIVE);                                    //Acknowledge delimiter
    send_decon_uint32(0xFFFF, 10);                      //EOF & interframe space filler
    return true;
  }
#elif defined(TEST_CASE_4)
  bool can_mac_tx_frame(CAN_FRAME* txFrame)
  {
    bool success = true;
    send(DOMINANT);                                     //SOF
    send_decon_uint32(txFrame->ID, 11);                 //Identifier
    send(DOMINANT);                                     //RTR
    send(DOMINANT);                                     //IDE
    send(DOMINANT);                                       //r0
    send_decon_uint32(txFrame->DLC, 4);                 //DLC
    send_decon_uint64(txFrame->Data, txFrame->DLC * 8); //Data depending on DLC
    send_decon_uint32(txFrame->CRC, 15);                //CRC
    send(RECESSIVE);                                    //CRC delimiter
    if(send(RECESSIVE) != DOMINANT)                     //Acknowledge
    {
      success = false;
    }
    send(RECESSIVE);                                    //Acknowledge delimiter
    send_decon_uint32(0xFFFF, 10);                      //EOF & interframe space filler
    return success;
  }
#elif defined(TEST_CASE_5)
  bool can_mac_tx_frame(CAN_FRAME* txFrame)
  {
    CAN_CHECK* current_state = (CAN_CHECK*)malloc(sizeof(CAN_CHECK));
    bool success = true;
    can_state_initial(current_state);
    current_state->crc_calc_check = true;
    send(DOMINANT, current_state);                                      //SOF
    send_decon_uint32(txFrame->ID, 11, current_state);                  //Identifier
    send(DOMINANT, current_state);                                      //RTR
    send(DOMINANT, current_state);                                      //IDE
    send(DOMINANT, current_state);                                      //r0
    send_decon_uint32(txFrame->DLC, 4, current_state);                  //DLC
    send_decon_uint64(txFrame->Data, txFrame->DLC * 8, current_state);  //Data depending on DLC
    if(((txFrame->CRC >> 31) & 1 ) == 1)
    {
      send_decon_uint32(txFrame->CRC,15, current_state);                //CRC
    }
    else
    {
      for(int i = 0 ; i < 15 ; ++i)
      {
        calc_curr_crc_bit(DOMINANT, current_state);
      }
      send_decon_uint32(current_state->crc_tracker,15, current_state);
    }
    send(RECESSIVE, current_state);                                     //CRC delimiter
    if(send(RECESSIVE, current_state) != DOMINANT)                      //Acknowledge
    {
      success = false;
    }
    send(RECESSIVE, current_state);                                     //Acknowledge delimiter
    send_decon_uint32(0xFFFF, 10, current_state);                       //EOF & interframe space filler
    free(current_state);
    return success;
  }
#elif defined(TEST_CASE_6)
  bool can_mac_tx_frame(CAN_FRAME* txFrame)
  {
    CAN_CHECK* current_state = (CAN_CHECK*)malloc(sizeof(CAN_CHECK));
    bool success = true;
    can_state_initial(current_state);
    current_state->crc_calc_check = true;
    send(DOMINANT, current_state);                                      //SOF
    send_decon_uint32(txFrame->ID, 11, current_state);                  //Identifier
    if(current_state->arbitration_error)
    {
      xil_printf("Arbitration failed, message id: 0x%x, DLC: 0x%x, Data: 0x%08x%08x, CRC: 0x%x hasn't been sent.\n", txFrame->ID, txFrame->DLC, (uint32_t)(txFrame->Data >> 32), (uint32_t)txFrame->Data, txFrame->CRC);
      free(current_state);
      return 0;
    }
    send(DOMINANT, current_state);                                      //RTR
    if(current_state->arbitration_error)
    {
      xil_printf("Arbitration failed, message id: 0x%x, DLC: 0x%x, Data: 0x%08x%08x, CRC: 0x%x hasn't been sent.\n", txFrame->ID, txFrame->DLC, (uint32_t)(txFrame->Data >> 32), (uint32_t)txFrame->Data, txFrame->CRC);
      free(current_state);
      return 0;
    }
    current_state->arbitration_check = false;
    send(DOMINANT, current_state);                                      //IDE
    send(DOMINANT, current_state);                                      //r0
    send_decon_uint32(txFrame->DLC, 4, current_state);                  //DLC
    send_decon_uint64(txFrame->Data, txFrame->DLC * 8, current_state);  //Data depending on DLC
    if(((txFrame->CRC >> 31) & 1 ) == 1)
    {
      send_decon_uint32(txFrame->CRC,15, current_state);                //CRC
    }
    else
    {
      for(int i = 0 ; i < 15 ; ++i)
      {
        calc_curr_crc_bit(DOMINANT, current_state);
      }
      send_decon_uint32(current_state->crc_tracker,15, current_state);
    }
    send(RECESSIVE, current_state);                                     //CRC delimiter
    if(send(RECESSIVE, current_state) != DOMINANT)                      //Acknowledge
    {
      success = false;
    }
    send(RECESSIVE, current_state);                                     //Acknowledge delimiter
    send_decon_uint32(0xFFFF, 10, current_state);                       //EOF & interframe space filler
    free(current_state);
    return success;
  }
#endif
#if defined(TEST_CASE_1) || defined(TEST_CASE_2) || defined(TEST_CASE_3)
  void can_mac_rx_frame(CAN_FRAME* rxFrame)
  {
    CAN_SYMBOL sym_data;
    uint32_t eof;
    wait_for_dominant();                                      //SOF
    receive_decon_uint32(&(rxFrame->ID), 11);                 //Identifier
    receive(&sym_data);                                       //RTR
    receive(&sym_data);                                       //IDE
    receive(&sym_data);                                       //r0
    receive_decon_uint32(&(rxFrame->DLC), 4);                 //DLC
    receive_decon_uint64(&(rxFrame->Data), rxFrame->DLC * 8); //Data depending on DLC
    receive_decon_uint32(&(rxFrame->CRC), 15);                //CRC
    receive(&sym_data);                                       //CRC delimiter
    receive(&sym_data);                                       //Acknowledge
    receive(&sym_data);                                       //Acknowledge delimiter
    receive_decon_uint32(&eof, 7);                            //EOF & interframe space filler
  }
#elif defined(TEST_CASE_4)
  void can_mac_rx_frame(CAN_FRAME* rxFrame)
  {
    CAN_SYMBOL sym_data;
    uint32_t eof;
    wait_for_dominant();                                      //SOF
    receive_decon_uint32(&(rxFrame->ID), 11);                 //Identifier
    receive(&sym_data);                                       //RTR
    receive(&sym_data);                                       //IDE
    receive(&sym_data);                                       //r0
    receive_decon_uint32(&(rxFrame->DLC), 4);                 //DLC
    receive_decon_uint64(&(rxFrame->Data), rxFrame->DLC * 8); //Data depending on DLC
    receive_decon_uint32(&(rxFrame->CRC), 15);                //CRC
    receive(&sym_data);                                       //CRC delimiter
  #ifdef ACK_CHECK                                            //Acknowledgement fail test ca
    send(DOMINANT);                  
  #else
    receive(&sym_data);                                       //Acknowledge
  #endif
    receive(&sym_data);                                       //Acknowledge delimiter
    receive_decon_uint32(&eof, 7);                            //EOF & interframe space filler
  }
#elif defined(TEST_CASE_5) || defined(TEST_CASE_6)
  void can_mac_rx_frame(CAN_FRAME* rxFrame)
  {
    bool tester = false;
    while(!tester)
    {
      CAN_SYMBOL sym_data;
      uint32_t eof;
      wait_for_dominant();                                                      //SOF
      CAN_CHECK* current_state = (CAN_CHECK*)malloc(sizeof(CAN_CHECK));
      current_state->crc_calc_check = true;
      receive_decon_uint32(&(rxFrame->ID), 11, current_state);                  //Identifier
      receive(DOMINANT, current_state);                                         //RTR
      receive(DOMINANT, current_state);                                         //IDE
      receive(DOMINANT, current_state);                                         //r0
      receive_decon_uint32(&(rxFrame->DLC), 4, current_state);                  //DLC
      receive_decon_uint64(&(rxFrame->Data), rxFrame->DLC * 8, current_state);  //Data depending on DLC
      receive_decon_uint32(&(rxFrame->CRC),15, current_state);                  //CRC
      //current_state->crc_calc_check = false;
      receive(&sym_data, current_state);                                        //CRC delimiter
      if(current_state->crc_calc_check == false)
      {
        xil_printf("CRC FAIL, Message id 0x%x, DLC 0x%0, Data: 0x%08x%08x, CRC: 0x%x FAIL .\n",rxFrame->ID , rxFrame->DLC,(uint32_t)(rxFrame->Data >>32),(uint32_t)rxFrame->Data, rxFrame->CRC);
        receive(&sym_data, current_state); // aCK
        tester = false;
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
#if defined(TEST_CASE_6)
  void case_6_sender(uint32_t ID, uint32_t DLC, uint32_t Data, uint32_t CRC)
  {
    uint64_t t = read_global_timer();
    xil_printf("%04u/%010u: CAN client %d %d running in TEST_CASE_6\n", (uint32_t)(t >> 32), (uint32_t)t, TILE_ID, PARTITION_ID);
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
    while(true)
    {
      CAN_FRAME* frame = (CAN_FRAME*)malloc(sizeof(CAN_FRAME));
      frame->ID = 0;
      frame->DLC = 0;
      frame->Data = 0;
      frame->CRC = 0;
      can_mac_rx_frame(frame);
      if (frame->ID != Period)
      {
        free(frame);
        continue;
      }
      xil_printf("CAN client %d %d Receiving frame id: 0x%x, DLC: 0x%x, Data: 0x%08x%08x, CRC: 0x%x\n", TILE_ID, PARTITION_ID, frame->ID, frame->DLC, (uint32_t)(frame->Data >> 32), (uint32_t)frame->Data, frame->CRC);
      free(frame);
    }
  }
#endif