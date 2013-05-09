/**
 * @file rtems/posix/key.inl
 */

/*  rtems/posix/key.inl
 *
 *  This include file contains the static inline implementation of the private 
 *  inlined routines for POSIX key's.
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: key.inl,v 1.12 2008/09/04 15:23:11 ralf Exp $
 */
 
#ifndef _RTEMS_POSIX_KEY_H
# error "Never use <rtems/posix/key.inl> directly; include <rtems/posix/key.h> instead."
#endif

#ifndef _RTEMS_POSIX_KEY_INL
#define _RTEMS_POSIX_KEY_INL
 
/*PAGE
 *
 *  _POSIX_Keys_Allocate
 */
 
RTEMS_INLINE_ROUTINE POSIX_Keys_Control *_POSIX_Keys_Allocate( void )
{
  return (POSIX_Keys_Control *) _Objects_Allocate( &_POSIX_Keys_Information );
}
 
/*PAGE
 *
 *  _POSIX_Keys_Free
 */
 
RTEMS_INLINE_ROUTINE void _POSIX_Keys_Free (
  POSIX_Keys_Control *the_key
)
{
  _Objects_Free( &_POSIX_Keys_Information, &the_key->Object );
}
 
/*PAGE
 *
 *  _POSIX_Keys_Get
 */
 
RTEMS_INLINE_ROUTINE POSIX_Keys_Control *_POSIX_Keys_Get (
  Objects_Id         id,
  Objects_Locations *location
)
{
  return (POSIX_Keys_Control *)
    _Objects_Get( &_POSIX_Keys_Information, id, location );
}
 
/*PAGE
 *
 *  _POSIX_Keys_Is_null
 */
 
RTEMS_INLINE_ROUTINE bool _POSIX_Keys_Is_null (
  POSIX_Keys_Control *the_key
)
{
  return !the_key;
}

#endif
/*  end of include file */

