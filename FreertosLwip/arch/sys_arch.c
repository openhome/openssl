#include "lwip/sys.h"
#include "lwip/stats.h"
#include <FreeRTOS/FreeRTOS.h>
#include <FreeRTOS/queue.h>


/*
 * This function is provided by the OS abstraction layer of Volkano
 */
sys_thread_t lwipCreateThread(const char *name, lwip_thread_fn thread, void *arg, int stacksize, int prio);


void sys_init()
{
}


err_t sys_mutex_new(sys_mutex_t *mutex)
{
    *mutex = xSemaphoreCreateMutex();

    if (*mutex == NULL) {
        SYS_STATS_INC(mutex.err);
        return ERR_MEM;
    }
    else {
#if configQUEUE_REGISTRY_SIZE > 0
        vQueueAddToRegistry(*mutex, (signed char *)"lwip_mutex");
#endif
        SYS_STATS_INC_USED(mutex);
        return ERR_OK;
    }
}


void sys_mutex_lock(sys_mutex_t *mutex)
{
    // Will never timeout, so don't bother checking result.
    xSemaphoreTake( *mutex, portMAX_DELAY );
}


void sys_mutex_unlock(sys_mutex_t *mutex)
{
    if ( xSemaphoreGive( *mutex ) != pdTRUE ) {
        LWIP_ASSERT("sys_mutex_unlock", 0);
    }
}


void sys_mutex_free(sys_mutex_t *mutex)
{
    vSemaphoreDelete(*mutex);
    SYS_STATS_DEC(mutex.used);
}


int sys_mutex_valid(sys_mutex_t *mutex)
{
    return ( *mutex != SYS_SEM_NULL ) ? 1 : 0;
}


void sys_mutex_set_invalid(sys_mutex_t *mutex)
{
    *mutex = SYS_SEM_NULL;
}


err_t sys_sem_new(sys_sem_t *sem, u8_t count)
{
    *sem = xSemaphoreCreateCounting(SYS_SEM_MAX, count);

    if (*sem == NULL) {
        SYS_STATS_INC(sem.err);
        return ERR_MEM;
    }
    else {
#if configQUEUE_REGISTRY_SIZE > 0
        vQueueAddToRegistry(*sem, (signed char *)"lwip_sem");
#endif
        SYS_STATS_INC_USED(sem);
        return ERR_OK;
    }
}


void sys_sem_signal(sys_sem_t *sem)
{
    if (xSemaphoreGive(*sem) != pdTRUE ){
        LWIP_ASSERT("sys_sem_signal", 0);
    }
}


u32_t sys_arch_sem_wait(sys_sem_t *sem, u32_t timeout)
{
    portTickType blockTicks, blockStart;

    if ( timeout != 0 )
    {
        blockTicks = (timeout + portTICK_RATE_MS - 1) / portTICK_RATE_MS;
    }
    else
    {
        blockTicks = portMAX_DELAY;
    }

    blockStart = xTaskGetTickCount();

    if ( xSemaphoreTake( *sem, blockTicks ) == pdTRUE )
        return ((xTaskGetTickCount() - blockStart) * portTICK_RATE_MS);
    else
        return SYS_ARCH_TIMEOUT;
}


void sys_sem_free(sys_sem_t *sem)
{
    vSemaphoreDelete(*sem);
    SYS_STATS_DEC(sem.used);
}


void sys_sem_set_invalid(sys_sem_t *sem)
{
    *sem = SYS_SEM_NULL;
}


int sys_sem_valid(sys_sem_t *sem)
{
    return ( *sem != SYS_SEM_NULL ) ? 1 : 0;
}


err_t sys_mbox_new(sys_mbox_t *mbox, int size)
{
    if ( size == 0 )
        size = SYS_DEFAULT_MBOX_SIZE;

    *mbox = xQueueCreate( size, sizeof(void*) );
    if (*mbox == NULL) {
        SYS_STATS_INC(mbox.err);
        return ERR_MEM;
    }
    else {
#if configQUEUE_REGISTRY_SIZE > 0
        vQueueAddToRegistry(*mbox, (signed char *)"lwip_mbox");
#endif
        SYS_STATS_INC_USED(mbox);
        return ERR_OK;
    }
}


void sys_mbox_post(sys_mbox_t *mbox, void *msg)
{
    xQueueSend( *mbox, &msg, portMAX_DELAY );
}


err_t sys_mbox_trypost(sys_mbox_t *mbox, void *msg)
{
    if ( xQueueSend( *mbox, &msg, 0 ) == pdTRUE )
        return ERR_OK;
    else
        return ERR_MEM;
}


u32_t sys_arch_mbox_fetch(sys_mbox_t *mbox, void **msg, u32_t timeout)
{
    portTickType blockTicks, blockStart;
    void *dummy;

    if (msg == NULL) {
        msg = &dummy;
    }

    if ( timeout != 0 )
    {
        blockTicks = (timeout + portTICK_RATE_MS - 1) / portTICK_RATE_MS;
    }
    else
    {
        blockTicks = portMAX_DELAY;
    }

    blockStart = xTaskGetTickCount();

    if ( xQueueReceive( *mbox, msg, blockTicks ) == pdTRUE ) {
        return ((xTaskGetTickCount() - blockStart) * portTICK_RATE_MS);
    }
    else {
        *msg = NULL;
        return SYS_ARCH_TIMEOUT;
    }
}


u32_t sys_arch_mbox_tryfetch(sys_mbox_t *mbox, void **msg)
{
    void *dummy;

    if (msg == NULL) {
        msg = &dummy;
    }

    if ( xQueueReceive( *mbox, msg, 0 ) == pdTRUE )
        return ERR_OK;
    else
        return SYS_MBOX_EMPTY;
}


void sys_mbox_free(sys_mbox_t *mbox)
{
    LWIP_ASSERT("sys_mbox_free: mbox empty", (uxQueueMessagesWaiting(*mbox) == 0));
    vQueueDelete(*mbox);
    SYS_STATS_DEC(mbox.used);
}


int sys_mbox_valid(sys_mbox_t *mbox)
{
    return ( *mbox != SYS_MBOX_NULL ) ? 1 : 0;
}


void sys_mbox_set_invalid(sys_mbox_t *mbox)
{
    *mbox = SYS_MBOX_NULL;
}


sys_thread_t sys_thread_new(const char *name, lwip_thread_fn thread, void *arg, int stacksize, int prio)
{
    if ( stacksize == 0 )
        stacksize = SYS_DEFAULT_THREAD_STACKSIZE;

    if ( prio == 0 )
        prio = SYS_DEFAULT_THREAD_PRIO;

    return lwipCreateThread((const char *) name, thread, arg, stacksize, prio);
}


sys_prot_t sys_arch_protect(void)
{
    portENTER_CRITICAL();
    return 0;
}


void sys_arch_unprotect(sys_prot_t pval)
{
    portEXIT_CRITICAL();
}


u32_t sys_now(void)
{
    return xTaskGetTickCount() * portTICK_RATE_MS;
}

