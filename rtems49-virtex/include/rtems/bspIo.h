/* bspIo.h
 *
 *  This include file contains declaration of interface that
 *  will be provided by the file contained in this directory.
 *
 *  COPYRIGHT (c) 1998 valette@crf.canon.fr
 *
 *  The license and distribution terms for this file may be
 *  found in found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: bspIo.h,v 1.7 2007/09/06 22:51:25 joel Exp $
 */
#ifndef _RTEMS_BSPIO_H
#define _RTEMS_BSPIO_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * All the functions declared as extern after this comment
 * MUST be implemented in each BSP. Using this function,
 * this directory contains shared code that export higher level
 * functionnality described after the next command.
 */
typedef void 	(*BSP_output_char_function_type) 	(char c);
typedef char 	(*BSP_polling_getchar_function_type) 	(void);

extern 	BSP_output_char_function_type 		BSP_output_char;
extern 	BSP_polling_getchar_function_type 	BSP_poll_char;

/*
 * All the function declared as extern after this comment
 * are available for each BSP by compiling and linking
 * the files contained in this directory PROVIDED definition
 * and initialisation of the previous variable are done.
 */
#include <stdarg.h>

extern void vprintk(const char *fmt, va_list ap);
extern void printk(const char *fmt, ...);

/*
 *  This routine is passed into RTEMS reporting functions
 *  that may have their output redirected.  In particular,
 *  the cpu usage, period usage, and stack usage reporting
 *  functions use this.  If the user provides their
 *  own "printf plugin", then they may redirect those reports
 *  as they see fit.
 */
extern int printk_plugin(void *context, const char *fmt, ...);
 
/*
 *  Type definition for function which can be plugged in to
 *  certain reporting routines to redirect the output
 */
typedef int (*rtems_printk_plugin_t)(void *, const char *format, ...);

#ifdef __cplusplus
}
#endif

#endif
