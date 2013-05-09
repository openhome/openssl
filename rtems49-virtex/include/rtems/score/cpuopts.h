/**
 * @file rtems/score/cpuopts.h
 */

/* target cpu dependent options file */
/* automatically generated -- DO NOT EDIT!! */
#ifndef _RTEMS_SCORE_CPUOPTS_H
#define _RTEMS_SCORE_CPUOPTS_H

/* if RTEMS_DEBUG is enabled */
/* #undef RTEMS_DEBUG */

/* if TLSF_HEAP is enabled */
#define TLSF_HEAP 1

/* if itron api is supported */
#define RTEMS_ITRON_API 1

/* if multiprocessing is enabled */
/* #undef RTEMS_MULTIPROCESSING */

/* if using newlib */
#define RTEMS_NEWLIB 1

/* if posix api is supported */
#define RTEMS_POSIX_API 1

/* if networking is enabled */
#define RTEMS_NETWORKING 1

/* to indicate RTEMS unix */
/* #undef RTEMS_UNIX */

/* to indicate RTEMS using RTEMS's unixlib */
/* #undef RTEMS_UNIXLIB */

/* if RTEMS uses stdint.h */
#define RTEMS_USES_STDINT_H 1

/* if RTEMS uses tar.h */
#define RTEMS_USES_TAR_H 1

/* RTEMS version string */
#define RTEMS_VERSION "4.9.0"

/* The size of a 'CPU_CONTEXT', as computed by sizeof */
/* #undef SIZEOF_CPU_CONTEXT */

/* disable nanosecond granularity for cpu usage statistics */
/* #undef __RTEMS_USE_TICKS_CPU_USAGE_STATISTICS__ */

/* disable nanosecond granularity for period statistics */
/* #undef __RTEMS_USE_TICKS_RATE_MONOTONIC_STATISTICS__ */

/* disable inlining _Thread_Enable_dispatch */
/* #undef __RTEMS_DO_NOT_INLINE_THREAD_ENABLE_DISPATCH__ */

/* disable inlining _Thread_Enable_dispatch */
/* #undef __RTEMS_DO_NOT_INLINE_CORE_MUTEX_SEIZE__ */

/* disable strict order mutex */
/* #undef __RTEMS_STRICT_ORDER_MUTEX__ */

/* Define to 1 if ada/gnat bindings are built-in */
/* #undef __RTEMS_ADA__ */

/* major version portion of an RTEMS release */
#define __RTEMS_MAJOR__ 4

/* minor version portion of an RTEMS release */
#define __RTEMS_MINOR__ 9

/* revision version portion of an RTEMS release */
#define __RTEMS_REVISION__ 0

#endif /* _RTEMS_SCORE_CPUOPTS_H */
