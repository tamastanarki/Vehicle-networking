#ifndef __CAN_PHY_H__
#define __CAN_PHY_H__

#include <stdint.h>

#define MAX_NUM_CAN_NODES 3

typedef enum {DOMINANT = 0, RECESSIVE = 1, UNSPECIFIED = 2} CAN_SYMBOL;
typedef enum {SYNC_SEG, PROP_SEG, PHASE_SEG_1, PHASE_SEG_2} BIT_SEGMENT;
typedef struct _can_frame {
  uint32_t ID;
  uint32_t DLC;
  uint64_t Data;
  uint32_t CRC;
} CAN_FRAME;

typedef struct
{
  int has_read_during_this_symbol;
  int has_written_during_this_symbol;
} CAN_NODE_STATE;

void can_phy_tx_symbol(CAN_SYMBOL symbol);
void can_phy_rx_symbol(CAN_SYMBOL *symbol);


// hack to link with right shared memory; cannot do this at run time
#define GPIO 0x81400000
#if(TILE_ID == 1)
#define MEM vep_private_mem01
#define GPIO_CONFIG 3
#define GPIO_RW 2
#endif
#if(TILE_ID == 2)
#define MEM vep_private_mem02
#define GPIO_CONFIG 1
#define GPIO_RW 0
#endif

#endif
