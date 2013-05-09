/**
 * @file  rtems/stdint.h
 *
 * Wrapper to <stdint.h>, switching to <inttypes.h> on systems 
 * only having <inttypes.h> (e.g. Solaris-5.7).
 */
 
/*
 * $Id: stdint.h,v 1.7 2006/12/13 07:21:25 ralf Exp $
 */

#ifndef _RTEMS_STDINT_H
#define _RTEMS_STDINT_H

#include <rtems/score/cpuopts.h>

#if RTEMS_USES_STDINT_H
#include <stdint.h>

#else
#include <inttypes.h>
#endif

#endif
