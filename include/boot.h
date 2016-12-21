#ifndef BOOT_H
#define BOOT_H

/* boot.h - Contains definitions and helper functions */

/* Watchdog timer register definitions */

#define WDT_BASE			0x44e35000
#define WDT_WSPR 			0x48
#define WDT_WWPS			0x34

/* GPIO1 clock register definitions */

#define CM_PER_BASE 			0x44e00000
#define CM_PER_GPIO1_CLKCTRL		0xac
#define ENABLE_MODULE			(2)

/* GPIO1 pinmux register definitions */

#define CONTROL_MODULE			0x44e10000
#define CONF_GPMC_A5			0x854
#define CONF_GPMC_A6			0x858
#define CONF_GPMC_A7			0x85c
#define CONF_GPMC_A8			0x860
#define MODE7				(7)

/* GPIO1 base register definitions */

#define GPIO1_BASE			0x4804c000
#define GPIO_CTRL			0x130
#define GPIO_OE				0x134
#define GPIO_SETDATAOUT			0x194
#define GPIO_CLEARDATAOUT		0x190
#define GPIO1_21			(21)
#define GPIO1_22                        (22)
#define GPIO1_23                        (23)
#define GPIO1_24                        (24)

/* Helper functions */

#ifndef ASM

void watchdog_disable(void)
{
	volatile unsigned int *wdt_reg = (unsigned int *)(WDT_BASE + WDT_WSPR);
	volatile unsigned int *wdt_status = (unsigned int *)(WDT_BASE + WDT_WWPS);

	*wdt_reg |= (unsigned int)0x0000AAAA;
	while(*wdt_status & (1 << 4));
	*wdt_reg |= (unsigned int)0x00005555;
	while(*wdt_status & (1 << 4));
}

void clk_enable(void)
{
	volatile unsigned int *clk_reg = (unsigned int *)(CM_PER_BASE + CM_PER_GPIO1_CLKCTRL);
	
	*clk_reg |= ENABLE_MODULE;
}

void pinmux_config(void)
{
	volatile unsigned int *pin_21 = (unsigned int *)(CONTROL_MODULE + CONF_GPMC_A5);
	volatile unsigned int *pin_22 = (unsigned int *)(CONTROL_MODULE + CONF_GPMC_A6);
	volatile unsigned int *pin_23 = (unsigned int *)(CONTROL_MODULE + CONF_GPMC_A7);
	volatile unsigned int *pin_24 = (unsigned int *)(CONTROL_MODULE + CONF_GPMC_A8);

	*pin_21 |= MODE7;
	*pin_22 |= MODE7;
	*pin_23 |= MODE7;
	*pin_24 |= MODE7;
}

void gpio_mod_enable(void)
{
	volatile unsigned int *gpio_reg = (unsigned int *)(GPIO1_BASE + GPIO_CTRL);

	*gpio_reg &= ~(1 << 0);
}

void gpio_set_out(void)
{
	volatile unsigned int *out_reg = (unsigned int *)(GPIO1_BASE + GPIO_OE);

	*out_reg &= ~(1 << GPIO1_21);
	*out_reg &= ~(1 << GPIO1_22);
	*out_reg &= ~(1 << GPIO1_23);
	*out_reg &= ~(1 << GPIO1_24);
}

void gpio_out_enable(void)
{
	volatile unsigned int *out_reg = (unsigned int *)(GPIO1_BASE + GPIO_SETDATAOUT);

	*out_reg |= (1 << GPIO1_21);
	*out_reg |= (1 << GPIO1_22);
	*out_reg |= (1 << GPIO1_23);
	*out_reg |= (1 << GPIO1_24);
}

void gpio_out_disable(void)
{
        volatile unsigned int *out_reg = (unsigned int *)(GPIO1_BASE + GPIO_CLEARDATAOUT);

        *out_reg |= (1 << GPIO1_21);
        *out_reg |= (1 << GPIO1_22);
        *out_reg |= (1 << GPIO1_23);
        *out_reg |= (1 << GPIO1_24);
}

void delay(volatile unsigned int count)
{
	while(count--);
}
#endif
#endif
