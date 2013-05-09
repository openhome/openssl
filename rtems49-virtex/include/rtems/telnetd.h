/*
 *  Original Author: Fernando RUIZ CASAS (fernando.ruiz@ctv.es)
 *  May 2001
 *  Reworked by Till Straumann and .h overhauled by Joel Sherrill.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 *  $Id: telnetd.h,v 1.7 2007/11/26 23:10:25 joel Exp $
 */

#ifndef _RTEMS_TELNETD_H
#define _RTEMS_TELNETD_H

#ifdef __cplusplus
extern "C" {
#endif	

/*
 *  Initialize the telnetd subsystem.
 *
 *  cmd              - function which is the "shell" telnetd invokes
 *  arg              - context pointer to cmd
 *  dontSpawn        - TRUE if telnetd takes over this task.
 *                     FALSE to create another task for the shell.
 *  stack            - stack size of spawned task 
 *  priority         - initial priority of spawned task
 *  askForPassword   - TRUE if telnetd asks for password
 *                     FALSE to invoke "cmd" with no password check.
 *                        This may be OK if "cmd" includes its own check.
 */
int rtems_telnetd_initialize(
  void               (*cmd)(char *, void *),
  void                *arg,
  int                  dontSpawn,
  size_t               stack,
  rtems_task_priority  priority,
  int                  askForPassword
);

#ifdef __cplusplus
}
#endif	

#endif
