//******************************************************************************
//*
//*      Project:   
//*
//*      File:      Root source file
//*
//*      Version 1.0
//*
//*      Copyright (c) 2016, Harry E. Zhurov
//*
//*
//------------------------------------------------------------------------------

#include <stdint.h>
#include <stdio.h>

#include <alt_mpu_registers.h>
#include <hps.h>
#include <socal.h>
#include <alt_l4wd.h>
#include <alt_watchdog.h>

#include    <alt_gpio.h>
#include    <alt_rstmgr.h>
#include    <hwlib.h>
#include    <alt_generalpurpose_io.h>


volatile uint32_t a = 1;
volatile uint32_t b = 2;
volatile uint32_t c = 10;
volatile uint32_t d = 0;

volatile float f1 = 3;
volatile float f2 = 2;

volatile double d1 = 10;
volatile double d2 = 20;

//---------------------------------------------------------------------------

void init();
uint32_t add(uint32_t x, uint32_t y);
uint32_t summ(uint32_t x);
uint32_t fib(uint32_t n);


//-----------------------------------------------------------------------------
/* This value must be written to the Counter Restart Register of the
 * peripheral watchdog timers to restart them. */
#define WDOG_RESET_KEY                       0x00000076
//#define ALT_RSTMGR_PERMODRST_L4WD0_SET_MSK   ALT_RSTMGR_PER1MODRST_WD0_SET_MSK

inline void reset_wdt0() { alt_write_word(ALT_L4WD0_WDT_CRR_ADDR, WDOG_RESET_KEY); }

//---------------------------------------------------------------------------
//
int main()
{
    alt_setbits_word(ALT_RSTMGR_PERMODRST_ADDR, ALT_RSTMGR_PERMODRST_L4WD0_SET_MSK);
    init();  
    
    for(;;)
    {
        c = a + b;
        a = add(b, c);
        //d2 *= d1 + f2;
        b = add(a, c);
        b += d;
        alt_setbits_word(ALT_GPIO1_SWPORTA_DR_ADDR, (1 << 24) | (1 << 25) | (1 << 26) | (1 << 27));
        d = summ(1000);
        //d = fib(1000);
        //f2 *= f1;
        
        //alt_wdog_reset(ALT_WDOG0);
        //reset_wdt0();
        
        
        alt_clrbits_word(ALT_GPIO1_SWPORTA_DR_ADDR, (1 << 24) | (1 << 25) | (1 << 26) | (1 << 27));
        b = summ(1000);
        
        
    }

}
//---------------------------------------------------------------------------
uint32_t add(uint32_t x, uint32_t y)
{
    uint32_t res = x + y;
    return res;
}
//---------------------------------------------------------------------------
uint32_t summ(uint32_t x)
{
    uint32_t s = 0;
    for(uint32_t i = 1; i <= x; ++i)
    {
        s += i;
    }
    return s;
}
//---------------------------------------------------------------------------
uint32_t fib(uint32_t n)
{
    uint32_t fn0 = 0;
    uint32_t fn1 = 1;
    uint32_t fn2 = 0;
    for(uint32_t i = 0; i < n; ++i)
    {
        fn2 = fn1 + fn0;
        fn0 = fn1;
        fn1  = fn2;
    }
    return fn2;
}
//---------------------------------------------------------------------------
void init()
{
    alt_gpio_init();
//  alt_gpio_port_datadir_set(ALT_GPIO_PORTB,
//                            ALT_GPIO_SWPORTA_DDR_GPIO_SWPORTA_DDR_SET_MSK,
//                            (1 << 24));
  
//  alt_replbits_word(ALT_GPIO1_SWPORTA_DDR_ADDR,
//                    ALT_GPIO_SWPORTA_DDR_GPIO_SWPORTA_DDR_SET_MSK,
//                    (1 << 24) | (1 << 25) | (1 << 26) | (1 << 27) );
  
    alt_setbits_word(ALT_GPIO1_SWPORTA_DDR_ADDR, (1 << 24) | (1 << 25) | (1 << 26) | (1 << 27));
}
//-----------------------------------------------------------------------------
/* This value must be written to the Counter Restart Register of the
 * peripheral watchdog timers to restart them. */
#define WDOG_RESET_KEY          0x00000076

#define ALT_WDOG_RST_WIDTH      8                       /* 8 or more MPU clock cycles */


bool cpu_wdog_in_gpt_mode(void)
{
    return !(alt_read_word(ALT_CPU_WDTGPT_TMR_BASE + ALT_WDOG_CTRL_REG_OFFSET) & ALT_WDOG_WDT_MODE);
}

static __inline bool cpu_wdog_in_wdt_mode(void)
{
    return (alt_read_word(ALT_CPU_WDTGPT_TMR_BASE + ALT_WDOG_CTRL_REG_OFFSET) & ALT_WDOG_WDT_MODE);
}

//-----------------------------------------------------------------------------
//ALT_STATUS_CODE alt_wdog_reset(ALT_WDOG_TIMER_t tmr_id)
//{
//    uint32_t                regdata;        /* data read */
//
//
//    if (tmr_id == ALT_WDOG_CPU)
//    {
//        regdata = alt_read_word(ALT_CPU_WDTGPT_TMR_BASE + ALT_WDOG_LOAD_REG_OFFSET);
//        alt_write_word(ALT_CPU_WDTGPT_TMR_BASE + ALT_WDOG_LOAD_REG_OFFSET, regdata);
//                /* verify operation when we have hardware,
//                 * the ARM documentation is somewhat vague here */
//
//        if (cpu_wdog_in_wdt_mode())
//        {
//            alt_write_word((ALT_CPU_WDTGPT_TMR_BASE + ALT_WDOG_RSTSTAT_REG_OFFSET), ALT_WDOG_RST_STAT_BIT);
//                      /* depending on current mode, clear the reset bit or... */
//        }
//        else
//        {
//            alt_write_word((ALT_CPU_WDTGPT_TMR_BASE + ALT_WDOG_INTSTAT_REG_OFFSET), ALT_WDOG_INT_STAT_BIT);
//                      /* ...clear the interrupt status bit by writing one to it */
//        }
//    }
//    else if ((tmr_id == ALT_WDOG0) || (tmr_id == ALT_WDOG0_INIT))
//    {
//        alt_write_word(ALT_L4WD0_WDT_CRR_ADDR, WDOG_RESET_KEY);
//            /*restarts the counter, also clears the watchdog timer interrupt */
//    }
//    else if ((tmr_id == ALT_WDOG1) || (tmr_id == ALT_WDOG1_INIT))
//    {
//        alt_write_word(ALT_L4WD1_WDT_CRR_ADDR, WDOG_RESET_KEY);
//            /*restarts the counter, also clears the watchdog timer interrupt */
//    }
//    else {return  ALT_E_BAD_ARG; }
//    return ALT_E_SUCCESS;
//}
//-----------------------------------------------------------------------------
/****************************************************************************************/
/* alt_gpio_init() initializes the GPIO modules                                         */
/****************************************************************************************/

ALT_STATUS_CODE alt_gpio_init(void)
{
    /* put GPIO modules into system manager reset if not already there */
    alt_gpio_uninit();
    /* release GPIO modules from system reset (w/ two-instruction delay) */
#ifdef soc_a10
    alt_replbits_word(ALT_RSTMGR_PER1MODRST_ADDR, 
                      ALT_RSTMGR_PER1MODRST_GPIO0_SET_MSK |
                      ALT_RSTMGR_PER1MODRST_GPIO1_SET_MSK |
                      ALT_RSTMGR_PER1MODRST_GPIO2_SET_MSK, 0);
#else
    alt_replbits_word(ALT_RSTMGR_PERMODRST_ADDR, ALT_RSTMGR_PERMODRST_GPIO0_SET_MSK |
                      ALT_RSTMGR_PERMODRST_GPIO1_SET_MSK |
                      ALT_RSTMGR_PERMODRST_GPIO2_SET_MSK, 0);
#endif
    return ALT_E_SUCCESS;
}


/****************************************************************************************/
/* alt_gpio_uninit() uninitializes the GPIO modules                                     */
/****************************************************************************************/

ALT_STATUS_CODE alt_gpio_uninit(void)
{
    /* put all GPIO modules into system manager reset */
#ifdef soc_a10
    alt_replbits_word(ALT_RSTMGR_PER1MODRST_ADDR, 
                      ALT_RSTMGR_PER1MODRST_GPIO0_SET_MSK |
                      ALT_RSTMGR_PER1MODRST_GPIO1_SET_MSK |
                      ALT_RSTMGR_PER1MODRST_GPIO2_SET_MSK,
                      ALT_GPIO_BITMASK);
#else
    alt_replbits_word(ALT_RSTMGR_PERMODRST_ADDR, ALT_RSTMGR_PERMODRST_GPIO0_SET_MSK |
                      ALT_RSTMGR_PERMODRST_GPIO1_SET_MSK |
                      ALT_RSTMGR_PERMODRST_GPIO2_SET_MSK,
                      ALT_GPIO_BITMASK);
#endif
    return ALT_E_SUCCESS;
}

//-----------------------------------------------------------------------------
/****************************************************************************************/
/* alt_gpio_port_datadir_set() sets the specified GPIO data bits to use the data        */
/* direction(s) specified. 0 = input (default). 1 = output.                             */
/****************************************************************************************/

//ALT_STATUS_CODE alt_gpio_port_datadir_set(ALT_GPIO_PORT_t gpio_pid,
//        uint32_t mask, uint32_t config)
//{
//    volatile uint32_t   *addr;
//
//    if ((mask & ~ALT_GPIO_BITMASK) || (config & ~ALT_GPIO_BITMASK))  { return ALT_E_ERROR; }
//    if (gpio_pid == ALT_GPIO_PORTA)      { addr = ALT_GPIO0_SWPORTA_DDR_ADDR; }
//    else if (gpio_pid == ALT_GPIO_PORTB) { addr = ALT_GPIO1_SWPORTA_DDR_ADDR; }
//    else if (gpio_pid == ALT_GPIO_PORTC) { addr = ALT_GPIO2_SWPORTA_DDR_ADDR; }
//    else { return ALT_E_BAD_ARG; }
//
//    alt_replbits_word(addr, mask, config);
//    return ALT_E_SUCCESS;
//}
//
//-----------------------------------------------------------------------------


