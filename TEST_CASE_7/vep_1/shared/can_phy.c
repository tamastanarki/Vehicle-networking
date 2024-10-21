#ifndef __CAN_PHY_C__
#define __CAN_PHY_C__

#include "can_phy.h"
#include "vep_private_memory.h"

void can_phy_tx_symbol(CAN_SYMBOL symbol)
{
  while (MEM->current_segment != PHASE_SEG_2);
  MEM->transmitting_symbols[PARTITION_ID] = symbol;
  MEM->node_states[PARTITION_ID].has_written_during_this_symbol = 1;
}

void can_phy_rx_symbol(CAN_SYMBOL *symbol)
{
  volatile uint32_t *gpio = (uint32_t *) GPIO;
  CAN_SYMBOL read_symbol = MEM->read_symbol;
  while ((read_symbol == UNSPECIFIED) ||
      (MEM->node_states[PARTITION_ID].has_read_during_this_symbol != 0) ||
      (MEM->node_states[PARTITION_ID].has_written_during_this_symbol != 0)) {
    read_symbol = MEM->read_symbol;
  }
  *symbol = (gpio[GPIO_RW] & 2) == 2; //read_symbol;
  MEM->node_states[PARTITION_ID].has_read_during_this_symbol = 1;
}


#endif
