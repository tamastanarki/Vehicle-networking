#ifndef __CAN_MAC_H__
#define __CAN_MAC_H__
#include <stdbool.h>

#include "can_phy.h"

/*** this file has to be written by students ***/

bool can_mac_tx_frame(CAN_FRAME* txFrame);
void can_mac_rx_frame(CAN_FRAME* rxFrame);

#endif
