/**
 * @file rtems/console.h
 */

/*  console.h
 *
 *  This file describes the Console Device Driver for all boards.
 *  This driver provides support for the standard C Library.
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: console.h,v 1.13 2008/05/27 10:32:04 thomas Exp $
 */

#ifndef _RTEMS_CONSOLE_H
#define _RTEMS_CONSOLE_H

#ifdef __cplusplus
extern "C" {
#endif

#define CONSOLE_DEVICE_NAME "/dev/console"

#define CONSOLE_DRIVER_TABLE_ENTRY \
  { console_initialize, console_open, console_close, \
    console_read, console_write, console_control }

rtems_device_driver console_initialize(
  rtems_device_major_number,
  rtems_device_minor_number,
  void *
);

rtems_device_driver console_open(
  rtems_device_major_number,
  rtems_device_minor_number,
  void *
);

rtems_device_driver console_close(
  rtems_device_major_number,
  rtems_device_minor_number,
  void *
);

rtems_device_driver console_read(
  rtems_device_major_number,
  rtems_device_minor_number,
  void *
);

rtems_device_driver console_write(
  rtems_device_major_number,
  rtems_device_minor_number,
  void *
);

rtems_device_driver console_control(
  rtems_device_major_number,
  rtems_device_minor_number,
  void *
);

#ifdef __cplusplus
}
#endif

#endif
/* end of include file */
