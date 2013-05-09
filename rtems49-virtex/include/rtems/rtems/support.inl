/**
 * @file rtems/rtems/support.inl
 *
 *  This include file contains the static inline implementation of all
 *  of the inlined routines specific to the RTEMS API.
 */

/*  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: support.inl,v 1.14 2008/09/04 17:43:18 ralf Exp $
 */
 
#ifndef _RTEMS_RTEMS_SUPPORT_H
# error "Never use <rtems/rtems/support.inl> directly; include <rtems/rtems/support.h> instead."
#endif

#ifndef _RTEMS_RTEMS_SUPPORT_INL
#define _RTEMS_RTEMS_SUPPORT_INL

/**
 *  @addtogroup ClassicSupport
 *  @{
 */

/**
 *  rtems_is_name_valid
 *
 *  This function returns TRUE if the name is valid, and FALSE otherwise.
 */
RTEMS_INLINE_ROUTINE bool rtems_is_name_valid (
  rtems_name name
)
{
  return ( name != 0 );
}

/**
 *  rtems_name_to_characters
 *
 *  This function breaks the object name into the four component
 *  characters C1, C2, C3, and C4.
 */
RTEMS_INLINE_ROUTINE void rtems_name_to_characters(
  rtems_name    name,
  char         *c1,
  char         *c2,
  char         *c3,
  char         *c4
)
{
  *c1 = (name >> 24) & 0xff;
  *c2 = (name >> 16) & 0xff;
  *c3 = (name >> 8) & 0xff;
  *c4 =  name & 0xff;
}

/**@}*/

#endif
/* end of include file */
