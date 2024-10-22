 #ifndef XIL_PRINTF_H
 #define XIL_PRINTF_H

#ifdef __cplusplus
extern "C" {
#endif

#include <ctype.h>
#include <string.h>
#include <stdarg.h>
#include "xil_types.h"
#if defined (__aarch64__) && HYP_GUEST && EL1_NONSECURE && XEN_USE_PV_CONSOLE
#include "xen_console.h"
#endif

/*----------------------------------------------------*/
/* Use the following parameter passing structure to   */
/* make xil_printf re-entrant.                        */
/*----------------------------------------------------*/

struct params_s;


/*---------------------------------------------------*/
/* The purpose of this routine is to output data the */
/* same as the standard printf function without the  */
/* overhead most run-time libraries involve. Usually */
/* the printf brings in many kilobytes of code and   */
/* that is unacceptable in most embedded systems.    */
/*---------------------------------------------------*/

typedef char8* charptr;
typedef s32 (*func_ptr)(int c);

/*                                                   */

void xil_printf( const char8 *ctrl1, ...);
void print( const char8 *ptr);
extern void outbyte (char8 c);
extern char8 inbyte(void);

// efficiently print a string
extern void print_string(char s[]);
// efficiently print a 64-bit number in the format "%X"
extern void print_uint64(uint64_t t);
// efficiently print a 64-bit time in the format "%03X/%010X: "
extern void print_time(uint64_t t);

#ifdef __cplusplus
}
#endif

#endif	/* end of protection macro */
