/**
 * @file rtems/ftpfs.h
 *
 * File Transfer Protocol client declarations
 *
 * Transfer file to/from remote host
 */

/*
 * (c) Copyright 2002
 * Thomas Doerfler
 * IMD Ingenieurbuero fuer Microcomputertechnik
 * Herbststr. 8
 * 82178 Puchheim, Germany
 * <Thomas.Doerfler@imd-systems.de>
 *
 * This code has been created after closly inspecting
 * "tftpdriver.c" from Eric Norum.
 *
 *  $Id: ftpfs.h,v 1.6 2008/08/01 05:07:41 ralf Exp $
 */

#ifndef _RTEMS_FTPFS_H
#define _RTEMS_FTPFS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <rtems/libio.h>

  /* create mount point and mount ftp file system */
extern int rtems_bsdnet_initialize_ftp_filesystem (void);

  /* FTP File sysem operations table. */
extern rtems_filesystem_operations_table rtems_ftp_ops;

#ifdef __cplusplus
}
#endif

#endif
