/**
 *  @file rtems/rtc.h
 *
 *  This file describes the Real-Time Clock driver for all boards.
 *  This driver provides support for the standard RTEMS routines
 *  that set the tod based on an RTC.
 */
 
/*
 *  COPYRIGHT (c) 1989-2001.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: rtc.h,v 1.4 2005/01/28 08:04:27 ralf Exp $
 */

#ifndef _RTEMS_RTC_H
#define _RTEMS_RTC_H

#ifdef __cplusplus
extern "C" {
#endif

#define RTC_DRIVER_TABLE_ENTRY \
  { rtc_initialize, NULL, NULL, NULL, NULL, NULL }

rtems_device_driver rtc_initialize(
  rtems_device_major_number,
  rtems_device_minor_number,
  void *
);

#ifdef __cplusplus
}
#endif

#endif
/* end of include file */
