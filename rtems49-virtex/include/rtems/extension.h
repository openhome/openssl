/**
 * @file rtems/extension.h
 */
 
/*
 *  This include file contains all the constants, structures, and
 *  prototypes associated with the User Extension Manager.  This manager
 *  provides a mechanism for manipulating sets of user-defined extensions.
 *
 *  Directives provided are:
 *
 *     + create user extension set
 *     + get ID of user extension set
 *     + delete user extension set
 *
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: extension.h,v 1.19 2007/05/21 23:19:20 joel Exp $
 */

#ifndef _RTEMS_EXTENSION_H
#define _RTEMS_EXTENSION_H

#ifndef SAPI_EXT_EXTERN
#define SAPI_EXT_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include <rtems/score/object.h>
#include <rtems/score/userext.h>
#include <rtems/rtems/status.h>
#include <rtems/rtems/types.h>

/*
 *  Extension related types
 */

typedef User_extensions_routine                   rtems_extension;
typedef User_extensions_thread_create_extension   rtems_task_create_extension;
typedef User_extensions_thread_delete_extension   rtems_task_delete_extension;
typedef User_extensions_thread_start_extension    rtems_task_start_extension;
typedef User_extensions_thread_restart_extension  rtems_task_restart_extension;
typedef User_extensions_thread_switch_extension   rtems_task_switch_extension;
typedef User_extensions_thread_post_switch_extension
                                              rtems_task_post_switch_extension;
typedef User_extensions_thread_begin_extension    rtems_task_begin_extension;
typedef User_extensions_thread_exitted_extension  rtems_task_exitted_extension;
typedef User_extensions_fatal_extension           rtems_fatal_extension;

typedef User_extensions_Table                     rtems_extensions_table;

/*
 *  The following defines the information control block used to manage
 *  this class of objects.
 */

SAPI_EXT_EXTERN Objects_Information  _Extension_Information;

/*
 *  The following records define the control block used to manage
 *  each extension.
 */

typedef struct {
  Objects_Control          Object;
  User_extensions_Control  Extension;
}   Extension_Control;

/*
 *  _Extension_Manager_initialization
 *
 *  DESCRIPTION:
 *
 *  This routine performs the initialization necessary for this manager.
 */

void _Extension_Manager_initialization(
  uint32_t   maximum_extensions
);

/*
 *  rtems_extension_create
 *
 *  DESCRIPTION:
 *
 *  This routine implements the rtems_extension_create directive.  The
 *  extension will have the name name.   The entry points of the
 *  routines which constitute this extension set are in EXTENSION_TABLE.
 *  It returns the id of the created extension in ID.
 */

rtems_status_code rtems_extension_create(
  rtems_name              name,
  rtems_extensions_table *extension_table,
  Objects_Id              *id
);

/*
 *  rtems_extension_ident
 *
 *  DESCRIPTION:
 *
 *  This routine implements the rtems_extension_ident directive.
 *  This directive returns the extension ID associated with name.
 *  If more than one extension is named name, then the extension
 *  to which the ID belongs is arbitrary.
 */

rtems_status_code rtems_extension_ident(
  rtems_name    name,
  Objects_Id   *id
);

/*
 *  rtems_extension_delete
 *
 *  DESCRIPTION:
 *
 *  This routine implements the rtems_extension_delete directive.  The
 *  extension indicated by ID is deleted.
 */

rtems_status_code rtems_extension_delete(
  Objects_Id id
);

#ifndef __RTEMS_APPLICATION__
#include <rtems/extension.inl>
#endif

#ifdef __cplusplus
}
#endif

#endif
/* end of include file */
