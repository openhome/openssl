/* Standard includes. */
#include <stdlib.h>
#include <string.h>
#include <reent.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"


#define portINITIAL_SPSR                ( ( portSTACK_TYPE ) 0x5f ) /* System mode, ARM mode, IRQ enabled, FIQ disabled. */
#define portINSTRUCTION_SIZE            ( ( portSTACK_TYPE ) 4 )

/* Constants required to handle critical sections. */
#define portNO_CRITICAL_NESTING         ( ( unsigned portLONG ) 0 )


/* ulCriticalNesting will get set to zero when the first task starts.  It
cannot be initialised to 0 as this will cause interrupts to be enabled
during the kernel initialisation process. */
volatile unsigned portLONG ulCriticalNesting = ( unsigned portLONG ) 9999;


void MX28SetupTimerInterrupt( void );

/*
 * Initialise the stack of a task to look exactly as if a call to
 * portSAVE_CONTEXT had been called.
 *
 * See header file for description.
 */
portSTACK_TYPE *pxPortInitialiseStack(portSTACK_TYPE *pxTopOfStack, pdTASK_CODE pxCode, void *pvParameters)
{
    portSTACK_TYPE *pxOriginalTOS;

    pxOriginalTOS = pxTopOfStack;

    /* To ensure asserts in tasks.c don't fail, although in this case the assert
    is not really required. */
    pxTopOfStack--;

    /* Setup the initial stack of the task.  The stack is set exactly as
    expected by the portRESTORE_CONTEXT() macro. */

    /* First on the stack is the return address - which in this case is the
    start of the task.  The offset is added to make the return address appear
    as it would within an IRQ ISR. */
    *pxTopOfStack = ( portSTACK_TYPE ) pxCode + portINSTRUCTION_SIZE;
    pxTopOfStack--;

    *pxTopOfStack = ( portSTACK_TYPE ) 0xaaaaaaaa;  /* R14 (LR) */
    pxTopOfStack--;
    *pxTopOfStack = ( portSTACK_TYPE ) pxOriginalTOS; /* Stack used when task starts goes in R13. */
    pxTopOfStack--;
    *pxTopOfStack = ( portSTACK_TYPE ) 0x12121212;  /* R12 (IP) */
    pxTopOfStack--;
    *pxTopOfStack = ( portSTACK_TYPE ) 0x00000000;  /* R11 (FP) */
    pxTopOfStack--;
    *pxTopOfStack = ( portSTACK_TYPE ) 0x10101010;  /* R10 */
    pxTopOfStack--;
    *pxTopOfStack = ( portSTACK_TYPE ) 0x09090909;  /* R9 */
    pxTopOfStack--;
    *pxTopOfStack = ( portSTACK_TYPE ) 0x08080808;  /* R8 */
    pxTopOfStack--;
    *pxTopOfStack = ( portSTACK_TYPE ) 0x07070707;  /* R7 */
    pxTopOfStack--;
    *pxTopOfStack = ( portSTACK_TYPE ) 0x06060606;  /* R6 */
    pxTopOfStack--;
    *pxTopOfStack = ( portSTACK_TYPE ) 0x05050505;  /* R5 */
    pxTopOfStack--;
    *pxTopOfStack = ( portSTACK_TYPE ) 0x04040404;  /* R4 */
    pxTopOfStack--;
    *pxTopOfStack = ( portSTACK_TYPE ) 0x03030303;  /* R3 */
    pxTopOfStack--;
    *pxTopOfStack = ( portSTACK_TYPE ) 0x02020202;  /* R2 */
    pxTopOfStack--;
    *pxTopOfStack = ( portSTACK_TYPE ) 0x01010101;  /* R1 */
    pxTopOfStack--;

    /* When the task starts is will expect to find the function parameter in R0. */
    *pxTopOfStack = ( portSTACK_TYPE ) pvParameters; /* R0 */
    pxTopOfStack--;

    /* The status register is set for system mode, with interrupts enabled. */
    *pxTopOfStack = ( portSTACK_TYPE ) portINITIAL_SPSR;

    pxTopOfStack--;

    /* Interrupt flags cannot always be stored on the stack and will
    instead be stored in a variable, which is then saved as part of the
    tasks context. */
    *pxTopOfStack = portNO_CRITICAL_NESTING;

    return pxTopOfStack;
}
/*-----------------------------------------------------------*/


void vPortISRStartFirstTask(void)
{
    /* Simply start the scheduler.  This is included here as it can only be
    called from ARM mode. */
    portRESTORE_CONTEXT();
}
/*-----------------------------------------------------------*/


/*-----------------------------------------------------------*/

portBASE_TYPE xPortStartScheduler(void)
{
    /* Start the timer that generates the tick ISR.  Interrupts are disabled
    here already. */
    MX28SetupTimerInterrupt();

    /* Start the first task. */
    vPortISRStartFirstTask();

    /* Should not get here! */
    return 0;
}
/*-----------------------------------------------------------*/


void vPortEndScheduler(void)
{
    /* It is unlikely that the ARM port will require this function as there
    is nothing to return to.  */
}
/*-----------------------------------------------------------*/


/*
 * Called by portYIELD() or taskYIELD() to manually force a context switch.
 *
 * When a context switch is performed from the task level the saved task
 * context is made to look as if it occurred from within the tick ISR.  This
 * way the same restore context function can be used when restoring the context
 * saved from the ISR or that saved from a call to vPortYieldProcessor.
 */
void vPortYieldProcessor(void) __attribute__((interrupt("SWI"), naked));

void vPortYieldProcessor(void)
{
    /* Within an IRQ ISR the link register has an offset from the true return
    address, but an SWI ISR does not.  Add the offset manually so the same
    ISR return code can be used in both cases. */
    __asm volatile ( "ADD       LR, LR, #4" );

    /* Perform the context switch.  First save the context of the current task. */
    portSAVE_CONTEXT();

    /* Find the highest priority task that is ready to run. */
    __asm volatile ( "bl vTaskSwitchContext" );

    /* Restore the context of the new task. */
    portRESTORE_CONTEXT();
}
/*-----------------------------------------------------------*/


void vPortEnterCritical(void)
{
    /* Disable interrupts first */
    portDISABLE_INTERRUPTS();

    /* Now interrupts are disabled ulCriticalNesting can be accessed
    directly.  Increment ulCriticalNesting to keep a count of how many times
    portENTER_CRITICAL() has been called. */
    ulCriticalNesting++;

}
/*-----------------------------------------------------------*/


void vPortExitCritical(void)
{
    if (ulCriticalNesting > portNO_CRITICAL_NESTING)
    {
        /* Decrement the nesting count as we are leaving a critical section. */
        ulCriticalNesting--;

        /* If the nesting level has reached zero then interrupts should be
        re-enabled. */
        if (ulCriticalNesting == portNO_CRITICAL_NESTING)
        {
            /* Enable interrupts */
            portENABLE_INTERRUPTS();
        }
    }
}
/*-----------------------------------------------------------*/


#if ( configTASK_REENT_SUPPORT == 1 )

typedef struct xPARAM_ITEM {
    xListItem item;
    void *name;
    void *value;
    void (*dtor)(void *);
} xParamItem;


typedef struct xTASK_DATA {
    struct _reent re;   // NOTE: this needs to be the first member of the structure
    xList pl;
} xTaskData;


portBASE_TYPE uxPortTaskAllocateHook(void** pvHook)
{
    xTaskData *dat;

    if (pvHook == NULL) {
        return pdFAIL;
    }

    dat = pvPortMalloc(sizeof(*dat));
    if (dat == NULL) {
        return pdFAIL;
    }

    _REENT_INIT_PTR(&dat->re);
    vListInitialise(&dat->pl);

    *pvHook = dat;
    return pdPASS;
}


void vPortTaskDeallocateHook(void** pvHook)
{
    if (pvHook != NULL) {
        if (*pvHook != NULL) {
            // First empty the parameter list
            xList *paramList = &((xTaskData *) *pvHook)->pl;
            xParamItem *param;
            while (!listLIST_IS_EMPTY(paramList)) {
                param = listGET_OWNER_OF_HEAD_ENTRY(paramList);
                uxListRemove(&param->item);
                if (param->dtor) {
                    param->dtor(param->value);
                }
                free(param);
            }

            // Now we can free the task data structure
            vPortFree(*pvHook);
            *pvHook = NULL;
        }
    }
}


void vPortTaskSwitchHook(void* pvHook)
{
    _impure_ptr = &((xTaskData *) pvHook)->re;
}


static xParamItem* pvFindParam(void *aName)
{
    xList *paramList = &((xTaskData *) _impure_ptr)->pl;
    xParamItem *param;
    portBASE_TYPE i;

    for (i = 0; i < listCURRENT_LIST_LENGTH(paramList); i++) {
        listGET_OWNER_OF_NEXT_ENTRY(param, paramList);
        if (param->name == aName) {
            return param;
        }
    }

    return NULL;
}


portBASE_TYPE xPortSetTaskParam(void *aName, const void *aValue, void (*dtor)(void *))
{
    xList *paramList;
    xParamItem *param;

    if (xTaskGetSchedulerState() == taskSCHEDULER_NOT_STARTED) {
        return pdFALSE;
    }

    paramList = &((xTaskData *) _impure_ptr)->pl;
    param = pvFindParam(aName);

    if (param) {
        param->value = (void *)aValue;
        param->dtor = dtor;
        return pdTRUE;
    }

    param = malloc(sizeof(*param));
    if (param == NULL) {
        return pdFALSE;
    }

    param->item.pvOwner = param;
    param->name = aName;
    param->value = (void *)aValue;
    param->dtor = dtor;

    vListInsertEnd(paramList, &param->item);

    return pdTRUE;
}


portBASE_TYPE xPortGetTaskParam(void * aName, void **aValue)
{
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
        xParamItem *param = pvFindParam(aName);

        if (param) {
            *aValue = param->value;
            return pdTRUE;
        }
    }

    return pdFALSE;
}


portBASE_TYPE xPortDelTaskParam(void * aName)
{
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
        xParamItem *param = pvFindParam(aName);

        if (param) {
            uxListRemove(&param->item);
            free(param);
            return pdTRUE;
        }
    }
    return pdFALSE;
}


#endif
