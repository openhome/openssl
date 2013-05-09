#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#define configUSE_PREEMPTION                1
#define configUSE_TICK_HOOK                 0
#define configTICK_RATE_HZ                  ( ( portTickType ) 100 )
#define configMINIMAL_STACK_SIZE            1024
#define configMAX_TASK_NAME_LEN             16
#define configUSE_TRACE_FACILITY            0
#define configUSE_16_BIT_TICKS              0
#define configIDLE_SHOULD_YIELD             1
#define configCHECK_FOR_STACK_OVERFLOW      2
#define configQUEUE_REGISTRY_SIZE           300
#define configMAX_PRIORITIES                152
#define configUSE_APPLICATION_TASK_TAG      1

/* Generate run time stats */
#define configGENERATE_RUN_TIME_STATS       1
#if configGENERATE_RUN_TIME_STATS == 1
#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS() vPortConfigureTimerForRunTimeStats()
#define portGET_RUN_TIME_COUNTER_VALUE()         uxPortGetRunTimeCounterValue()
#endif

/* Power saving (use either the idle hook or the tickless idle mode */
#define configUSE_IDLE_HOOK                 0
#define configUSE_TICKLESS_IDLE             0
#if configUSE_TICKLESS_IDLE == 1
#define configEXPECTED_IDLE_TIME_BEFORE_SLEEP       2
#define portSUPPRESS_TICKS_AND_SLEEP( xIdleTime )   vPortSuppressTicksAndSleep( xIdleTime )
#endif

/* Mutex and semaphore definitions */
#define configUSE_MUTEXES                   1
#define configUSE_COUNTING_SEMAPHORES       1
#define configUSE_RECURSIVE_MUTEXES         1

/* Co-routine definitions */
#define configUSE_CO_ROUTINES               0
#define configMAX_CO_ROUTINE_PRIORITIES     1

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */
#define INCLUDE_vTaskPrioritySet            1
#define INCLUDE_uxTaskPriorityGet           1
#define INCLUDE_vTaskDelete                 1
#define INCLUDE_vTaskSuspend                1
#define INCLUDE_vTaskDelayUntil             1
#define INCLUDE_vTaskDelay                  1
#define INCLUDE_xTaskGetSchedulerState      1

#define INCLUDE_xQueueGetMutexHolder        1

#define configTASK_REENT_SUPPORT            1

#ifdef __cplusplus
extern "C" {
#endif
void vFreertosAssertHandler(const char *aFile, int aLine);
#ifdef __cplusplus
}
#endif

#define configASSERT(x)                     do { if (!(x)) vFreertosAssertHandler(__FILE__, __LINE__); } while (0);

#endif /* FREERTOSCONFIG_H_ */
