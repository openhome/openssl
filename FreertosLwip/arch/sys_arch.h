#ifndef __ARCH_SYS_ARCH_H__
#define __ARCH_SYS_ARCH_H__

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"


#define SYS_MBOX_NULL                   NULL
#define SYS_SEM_NULL                    NULL

#define SYS_SEM_MAX                     2000

#define SYS_DEFAULT_MBOX_SIZE           128
#define SYS_DEFAULT_THREAD_STACKSIZE    16384
#define SYS_DEFAULT_THREAD_PRIO         142


typedef xSemaphoreHandle    sys_sem_t;
typedef xSemaphoreHandle    sys_mutex_t;
typedef xQueueHandle        sys_mbox_t;
typedef u32_t               sys_thread_t;
typedef u32_t               sys_prot_t;

#endif

