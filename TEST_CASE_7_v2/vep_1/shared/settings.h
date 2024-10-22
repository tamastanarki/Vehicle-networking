/*
 * CAN PHY driver debugging level
 * def bit meaning
 * LED 0   received/resolved symbol color LED 0 on/of
 * RX  1   show received/resolved symbol on the bus
 * TX  2   show symbols transmitted on the bus
 * CLK 3   show all sub-symbol timing information
 */
#define DEBUG_CAN_PHY_LED 1
#define DEBUG_CAN_PHY_RX 2
#define DEBUG_CAN_PHY_TX 4
#define DEBUG_CAN_PHY_CLK 8
#define DEBUG_CAN_PHY   (DEBUG_CAN_PHY_LED) /*| (DEBUG_CAN_PHY_RX) */

/* exported from can-phy-driver.c - do not modify */
#define SYMBOL_LENGTH_CYCLES ((uint64_t)20423000)
#define FRAME_LENGTH_CYCLES ((uint64_t)(SYMBOL_LENGTH_CYCLES*135))

/* define the test case you want to run */
#define TEST_CASE_7

#define PERIOD0 (FRAME_LENGTH_CYCLES*3)
#define PERIOD1 (FRAME_LENGTH_CYCLES*4)
#define PERIOD2 (FRAME_LENGTH_CYCLES*6)

#define PRIO_PERIOD0 3
#define PRIO_PERIOD1 13
#define PRIO_PERIOD2 14

/* test case 0
 * define your parameters if needed
 */
#ifdef TEST_CASE_4
  //#define ACK_CHECK
#endif

#ifdef TEST_CASE_5
	#define CRC_BASE 0x4599
#endif

#ifdef TEST_CASE_6
	#define CRC_BASE 0x4599
#endif

#ifdef TEST_CASE_7
	#define CRC_BASE 0x4599
#endif