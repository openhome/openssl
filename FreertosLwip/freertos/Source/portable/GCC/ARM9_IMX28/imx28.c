#include <stdlib.h>
#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"

#define readl(a)        (*(volatile uint32_t *)(a))
#define writel(v,a)     (*(volatile uint32_t *)(a) = (v))

#define XTAL_FREQ_HZ            24000000

#define REGSET                  4
#define REGCLR                  8

#define CLKCTRL_XBUS            0x80040070

/* CLKCTRL_XBUS register fields */
#define BP_CLKCTRL_XBUS_DIV     0x0
#define BM_CLKCTRL_XBUS_DIV     0x3f

/* We use TIMER0 to generate the system clock tick */
#define TIMROT_TIMCTRL0         0x80068020
#define TIMROT_RUNNING_COUNT0   0x80068030
#define TIMROT_FIXED_COUNT0     0x80068040

#define TIMROT_TIMCTRL1         0x80068060
#define TIMROT_RUNNING_COUNT1   0x80068070
#define TIMROT_FIXED_COUNT1     0x80068080

#define HW_RTC_MILLISECONDS     0x80056020


/* TIMCTRL0 register fields */
#define BM_TIMCTRL_IRQ                  (1 << 15)
#define BM_TIMCTRL_IRQ_EN               (1 << 14)
#define BM_TIMCTRL_UPDATE               (1 << 7)
#define BM_TIMCTRL_RELOAD               (1 << 6)
#define BF_TIMCTRL_DIV_BY_1             (0 << 4)
#define BF_TIMCTRL_DIV_BY_8             (3 << 4)
#define BM_TIMCTRL_SELECT               0xf
#define BF_TIMCTRL_SELECT_NEVER_TICK    0x0
#define BF_TIMCTRL_SELECT_TICK_ALWAYS   0xf


static unsigned long ulTimerReloadValueForOneTick;

#if configUSE_TICKLESS_IDLE == 1
/*
 * The maximum number of tick periods that can be suppressed is limited by the
 * 32 bit resolution of the tick timer.
 */
static unsigned long ulMaximumPossibleSuppressedTicks;

#endif /* configUSE_TICKLESS_IDLE */


void vPortSysTickHandler(void)
{
    /* Clear the timer IRQ */
    writel(BM_TIMCTRL_IRQ, TIMROT_TIMCTRL0 + REGCLR);

    /* Increment the RTOS tick count, then look for the highest priority
       task that is ready to run. */
    vTaskIncrementTick();

#if configUSE_PREEMPTION == 1
    vTaskSwitchContext();
#endif
}


void MX28SetupTimerInterrupt( void )
{
    /*
     * The timer clock rate is derived from the APBX clock.
     * In addition the timer clock is prescaled by a factor of 8
     */
    unsigned long ulTimerTickRate = ( XTAL_FREQ_HZ / ((readl(CLKCTRL_XBUS) & BM_CLKCTRL_XBUS_DIV) >> BP_CLKCTRL_XBUS_DIV) ) >> 3;

    ulTimerReloadValueForOneTick = ulTimerTickRate / configTICK_RATE_HZ;

#if configUSE_TICKLESS_IDLE == 1
    ulMaximumPossibleSuppressedTicks = (0xFFFFFFFF / ulTimerReloadValueForOneTick) - 1;
#endif

    /* Set the reload value for the timer */
    writel(ulTimerReloadValueForOneTick - 1, TIMROT_FIXED_COUNT0);

    /* Configure the timer to generate IRQs and auto-reload */
    writel( BM_TIMCTRL_IRQ_EN | BM_TIMCTRL_RELOAD | BM_TIMCTRL_UPDATE |
            BF_TIMCTRL_DIV_BY_8 | BF_TIMCTRL_SELECT_TICK_ALWAYS,
            TIMROT_TIMCTRL0);
}


#if (configUSE_IDLE_HOOK == 1) || (configUSE_TICKLESS_IDLE == 1)

static void wfi()
{
    /* Put the CPU in WFI mode */
    __asm volatile(
            "mcr p15, 0, %0, c7, c0, 4 \n"
            "nop                         "
            :
            : "r" (0)
        );
}

#endif

# if configUSE_IDLE_HOOK == 1

void vApplicationIdleHook(void)
{
    wfi();
}

#endif


#if configUSE_TICKLESS_IDLE == 1

void vPortTicklessStandbyHandler(void)
{
}


static void SysTickDisable(void)
{
    /* Disable the Timer 0 IRQ */
    writel(BM_TIMCTRL_IRQ_EN, TIMROT_TIMCTRL0 + REGCLR);
}


static void SysTickEnable(void)
{
    /* Clear any pending timer IRQs */
    writel(BM_TIMCTRL_IRQ, TIMROT_TIMCTRL0 + REGCLR);

    /* Re-enable the Timer 0 IRQ */
    writel(BM_TIMCTRL_IRQ_EN, TIMROT_TIMCTRL0 + REGSET);
}


void vPortSuppressTicksAndSleep( portTickType xExpectedIdleTime )
{
    portTickType xModifiableIdleTime;
    portBASE_TYPE timerTriggered;

    /* Disable interrupts */
    portDISABLE_INTERRUPTS();

    if ( eTaskConfirmSleepModeStatus() == eAbortSleep ) {
        /* We cannot go to sleep so enable interrupts again */
        portENABLE_INTERRUPTS();
    }
    else {
        unsigned long ulWaitCount, ulCompleteTickPeriods;

        /* Disable the SysTick IRQ */
        SysTickDisable();

        /* Make sure the reload value does not overflow the counter. */
        if ( xExpectedIdleTime > ulMaximumPossibleSuppressedTicks )
        {
            xExpectedIdleTime = ulMaximumPossibleSuppressedTicks;
        }

        /*
         * Calculate the load value for Timer 1. This should cause the timer to
         * generate an IRQ half way through the last system tick period, before
         * the System Tick needs to be enabled again.
         */
        ulWaitCount = ( ulTimerReloadValueForOneTick * ( xExpectedIdleTime - 1UL ) );
        ulWaitCount += (int)(readl(TIMROT_RUNNING_COUNT0) - ulTimerReloadValueForOneTick/2);

        /*
         * Configure and start Timer 1
         */
        writel(~0, TIMROT_FIXED_COUNT1);
        writel(BM_TIMCTRL_IRQ, TIMROT_TIMCTRL1 + REGCLR);
        writel(BM_TIMCTRL_IRQ_EN | BM_TIMCTRL_UPDATE |
               BF_TIMCTRL_DIV_BY_8 | BF_TIMCTRL_SELECT_TICK_ALWAYS,
               TIMROT_TIMCTRL1);
        writel(ulWaitCount, TIMROT_FIXED_COUNT1);

        /*
         * Sleep until something happens.  configPRE_SLEEP_PROCESSING() can
         * set its parameter to 0 to indicate that its implementation contains
         * its own wait for interrupt or wait for event instruction, and so wfi
         * should not be executed again.  However, the original expected idle
         * time variable must remain unmodified, so a copy is taken.
         */
        xModifiableIdleTime = xExpectedIdleTime;
        configPRE_SLEEP_PROCESSING( xModifiableIdleTime );
        if ( xModifiableIdleTime > 0 ) {
            wfi();
        }

        /* Stop Timer 1 */
        writel(BF_TIMCTRL_SELECT_NEVER_TICK, TIMROT_TIMCTRL1);
        /* Disable Timer 1 IRQ */
        writel(BM_TIMCTRL_IRQ_EN, TIMROT_TIMCTRL1 + REGCLR);

        /* If Timer 1 has triggered, then clear the pending IRQ */
        timerTriggered = readl(TIMROT_TIMCTRL1) & BM_TIMCTRL_IRQ;
        if (timerTriggered) {
            writel(BM_TIMCTRL_IRQ, TIMROT_TIMCTRL1 + REGCLR);
        }

        configPOST_SLEEP_PROCESSING( xExpectedIdleTime );

        portENABLE_INTERRUPTS();

        ulCompleteTickPeriods = xExpectedIdleTime - 1;
        if (!timerTriggered) {
            /*
             * We were woken up early by some other interrupt. The running time of the timer tells us how many
             * tick periods we did not manage to sleep.
             */
            ulCompleteTickPeriods -= (readl(TIMROT_RUNNING_COUNT1) + ulTimerReloadValueForOneTick/2) / ulTimerReloadValueForOneTick;
        }

        /* Increment the kernel tick count */
        vTaskStepTick( ulCompleteTickPeriods );

        /* Enable the SysTick IRQ */
        SysTickEnable();
    }
}

#endif   /* configUSE_TICKLESS_IDLE */


#if configGENERATE_RUN_TIME_STATS == 1

void vPortConfigureTimerForRunTimeStats()
{
    writel(0, HW_RTC_MILLISECONDS);
}

portTickType uxPortGetRunTimeCounterValue()
{
    return readl(HW_RTC_MILLISECONDS);
}

#endif
