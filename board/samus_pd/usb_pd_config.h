/* Copyright (c) 2014 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "adc.h"
#include "chip/stm32/registers.h"
#include "gpio.h"

/* USB Power delivery board configuration */

#ifndef __USB_PD_CONFIG_H
#define __USB_PD_CONFIG_H

/* Port and task configuration */
#define PD_PORT_COUNT 2
#define PORT_TO_TASK_ID(port) ((port) ? TASK_ID_PD_C1 : TASK_ID_PD_C0)
#define TASK_ID_TO_PORT(id)   ((id) == TASK_ID_PD_C0 ? 0 : 1)

/* Timer selection for baseband PD communication */
#define TIM_CLOCK_PD_TX_C0 17
#define TIM_CLOCK_PD_RX_C0 1
#define TIM_CLOCK_PD_TX_C1 14
#define TIM_CLOCK_PD_RX_C1 3

#define TIM_CLOCK_PD_TX(p) ((p) ? TIM_CLOCK_PD_TX_C1 : TIM_CLOCK_PD_TX_C0)
#define TIM_CLOCK_PD_RX(p) ((p) ? TIM_CLOCK_PD_RX_C1 : TIM_CLOCK_PD_RX_C0)

/* Timer channel */
#define TIM_RX_CCR_C0 1
#define TIM_RX_CCR_C1 1
#define TIM_TX_CCR_C0 1
#define TIM_TX_CCR_C1 1

/* RX timer capture/compare register */
#define TIM_CCR_C0 (&STM32_TIM_CCRx(TIM_CLOCK_PD_RX_C0, TIM_RX_CCR_C0))
#define TIM_CCR_C1 (&STM32_TIM_CCRx(TIM_CLOCK_PD_RX_C1, TIM_RX_CCR_C1))
#define TIM_RX_CCR_REG(p) ((p) ? TIM_CCR_C1 : TIM_CCR_C0)

/* TX and RX timer register */
#define TIM_REG_TX_C0 (STM32_TIM_BASE(TIM_CLOCK_PD_TX_C0))
#define TIM_REG_RX_C0 (STM32_TIM_BASE(TIM_CLOCK_PD_RX_C0))
#define TIM_REG_TX_C1 (STM32_TIM_BASE(TIM_CLOCK_PD_TX_C1))
#define TIM_REG_RX_C1 (STM32_TIM_BASE(TIM_CLOCK_PD_RX_C1))
#define TIM_REG_TX(p) ((p) ? TIM_REG_TX_C1 : TIM_REG_TX_C0)
#define TIM_REG_RX(p) ((p) ? TIM_REG_RX_C1 : TIM_REG_RX_C0)

/* use the hardware accelerator for CRC */
#define CONFIG_HW_CRC

/* TX uses SPI1 on PB3-4 for port C1, SPI2 on PB 13-14 for port C0 */
#define SPI_REGS(p) ((p) ? STM32_SPI1_REGS : STM32_SPI2_REGS)
static inline void spi_enable_clock(int port)
{
	if (port == 0)
		STM32_RCC_APB1ENR |= STM32_RCC_PB1_SPI2;
	else
		STM32_RCC_APB2ENR |= STM32_RCC_PB2_SPI1;
}

/* DMA for transmit uses DMA CH7 for C0 and DMA_CH3 for C1 */
#define DMAC_SPI_TX(p) ((p) ? STM32_DMAC_CH3 : STM32_DMAC_CH7)

/* RX uses COMP1 and TIM1 CH1 on port C0 and COMP2 and TIM3_CH1 for port C1*/
#define CMP1OUTSEL STM32_COMP_CMP1OUTSEL_TIM1_IC1
#define CMP2OUTSEL STM32_COMP_CMP2OUTSEL_TIM3_IC1

#define TIM_TX_CCR_IDX(p) ((p) ? TIM_TX_CCR_C1 : TIM_TX_CCR_C0)
#define TIM_RX_CCR_IDX(p) ((p) ? TIM_RX_CCR_C1 : TIM_RX_CCR_C0)
#define TIM_CCR_CS  1
#define EXTI_COMP_MASK(p) ((p) ? (1<<22) : (1 << 21))
#define IRQ_COMP STM32_IRQ_COMP
/* triggers packet detection on comparator falling edge */
#define EXTI_XTSR STM32_EXTI_FTSR

/* DMA for receive uses DMA_CH2 for C0 and DMA_CH6 for C1 */
#define DMAC_TIM_RX(p) ((p) ? STM32_DMAC_CH6 : STM32_DMAC_CH2)

/* the pins used for communication need to be hi-speed */
static inline void pd_set_pins_speed(int port)
{
	if (port == 0) {
		/* 40 MHz pin speed on SPI PB13/14 */
		STM32_GPIO_OSPEEDR(GPIO_B) |= 0x3C000000;
		/* 40 MHz pin speed on TIM17_CH1 (PE1) */
		STM32_GPIO_OSPEEDR(GPIO_E) |= 0x0000000C;
	} else {
		/* 40 MHz pin speed on SPI PB3/4 */
		STM32_GPIO_OSPEEDR(GPIO_B) |= 0x000003C0;
		/* 40 MHz pin speed on TIM14_CH1 (PB1) */
		STM32_GPIO_OSPEEDR(GPIO_B) |= 0x0000000C;
	}
}

/* Reset SPI peripheral used for TX */
static inline void pd_tx_spi_reset(int port)
{
	if (port == 0) {
		/* Reset SPI2 */
		STM32_RCC_APB1RSTR |= (1 << 14);
		STM32_RCC_APB1RSTR &= ~(1 << 14);
	} else {
		/* Reset SPI1 */
		STM32_RCC_APB2RSTR |= (1 << 12);
		STM32_RCC_APB2RSTR &= ~(1 << 12);
	}
}

/* Drive the CC line from the TX block */
static inline void pd_tx_enable(int port, int polarity)
{
	if (port == 0) {
		/* put SPI function on TX pin */
		if (polarity) /* PD3 is SPI2 MISO */
			gpio_set_alternate_function(GPIO_D, 0x0008, 1);
		else /* PB14 is SPI2 MISO */
			gpio_set_alternate_function(GPIO_B, 0x4000, 0);

		/* set the low level reference */
		gpio_set_level(GPIO_USB_C0_CC_TX_EN, 1);
	} else {
		/* put SPI function on TX pin */
		if (polarity) /* PE14 is SPI1 MISO */
			gpio_set_alternate_function(GPIO_E, 0x4000, 1);
		else /* PB4 is SPI1 MISO */
			gpio_set_alternate_function(GPIO_B, 0x0010, 0);

		/* set the low level reference */
		gpio_set_level(GPIO_USB_C1_CC_TX_EN, 1);
	}
}

/* Put the TX driver in Hi-Z state */
static inline void pd_tx_disable(int port, int polarity)
{
	if (port == 0) {
		/* output low on SPI TX to disable the FET */
		if (polarity) /* PD3 is SPI2 MISO */
			STM32_GPIO_MODER(GPIO_D) = (STM32_GPIO_MODER(GPIO_D)
						   & ~(3 << (2*3)))
						   |  (1 << (2*3));
		else /* PB14 is SPI2 MISO */
			STM32_GPIO_MODER(GPIO_B) = (STM32_GPIO_MODER(GPIO_B)
						   & ~(3 << (2*14)))
						   |  (1 << (2*14));

		/* put the low level reference in Hi-Z */
		gpio_set_level(GPIO_USB_C0_CC_TX_EN, 0);
	} else {
		/* output low on SPI TX to disable the FET */
		if (polarity) /* PE14 is SPI1 MISO */
			STM32_GPIO_MODER(GPIO_E) = (STM32_GPIO_MODER(GPIO_E)
						   & ~(3 << (2*14)))
						   |  (1 << (2*14));
		else /* PB4 is SPI1 MISO */
			STM32_GPIO_MODER(GPIO_B) = (STM32_GPIO_MODER(GPIO_B)
						   & ~(3 << (2*4)))
						   |  (1 << (2*4));

		/* put the low level reference in Hi-Z */
		gpio_set_level(GPIO_USB_C1_CC_TX_EN, 0);
	}
}

/* we know the plug polarity, do the right configuration */
static inline void pd_select_polarity(int port, int polarity)
{
	uint32_t val = STM32_COMP_CSR;

	/* Use window mode so that COMP1 and COMP2 share non-inverting input */
	val |= STM32_COMP_CMP1EN | STM32_COMP_CMP2EN | STM32_COMP_WNDWEN;

	if (port == 0) {
		/* use the right comparator inverted input for COMP1 */
		STM32_COMP_CSR = (val & ~STM32_COMP_CMP1INSEL_MASK) |
				 (polarity ? STM32_COMP_CMP1INSEL_INM4
					   : STM32_COMP_CMP1INSEL_INM6);
	} else {
		/* use the right comparator inverted input for COMP2 */
		STM32_COMP_CSR = (val & ~STM32_COMP_CMP2INSEL_MASK) |
				 (polarity ? STM32_COMP_CMP2INSEL_INM5
					   : STM32_COMP_CMP2INSEL_INM6);
	}
}

/* Initialize pins used for TX and put them in Hi-Z */
static inline void pd_tx_init(void)
{
	gpio_config_module(MODULE_USB_PD, 1);
}

static inline void pd_set_host_mode(int port, int enable)
{
	if (port == 0) {
		if (enable) {
			/* We never charging in power source mode */
			gpio_set_level(GPIO_USB_C0_CHARGE_EN_L, 1);
			/* High-Z is used for host mode. */
			gpio_set_level(GPIO_USB_C0_CC1_ODL, 1);
			gpio_set_level(GPIO_USB_C0_CC2_ODL, 1);
		} else {
			/* Kill VBUS power supply */
			gpio_set_level(GPIO_USB_C0_5V_EN, 0);
			/* Pull low for device mode. */
			gpio_set_level(GPIO_USB_C0_CC1_ODL, 0);
			gpio_set_level(GPIO_USB_C0_CC2_ODL, 0);
			/* Let charge_manager decide to enable the port */
		}
	} else {
		if (enable) {
			/* We never charging in power source mode */
			gpio_set_level(GPIO_USB_C1_CHARGE_EN_L, 1);
			/* High-Z is used for host mode. */
			gpio_set_level(GPIO_USB_C1_CC1_ODL, 1);
			gpio_set_level(GPIO_USB_C1_CC2_ODL, 1);
		} else {
			/* Kill VBUS power supply */
			gpio_set_level(GPIO_USB_C1_5V_EN, 0);
			/* Pull low for device mode. */
			gpio_set_level(GPIO_USB_C1_CC1_ODL, 0);
			gpio_set_level(GPIO_USB_C1_CC2_ODL, 0);
			/* Let charge_manager decide to enable the port */
		}
	}
}

/**
 * Initialize various GPIOs and interfaces to safe state at start of pd_task.
 *
 * These include:
 *   VBUS, charge path based on power role.
 *   Physical layer CC transmit.
 *   VCONNs disabled.
 *
 * @param port        USB-C port number
 * @param power_role  Power role of device
 */
static inline void pd_config_init(int port, uint8_t power_role)
{
	/*
	 * Set CC pull resistors, and charge_en and vbus_en GPIOs to match
	 * the initial role.
	 */
	pd_set_host_mode(port, power_role);

	/* Initialize TX pins and put them in Hi-Z */
	pd_tx_init();

	/* Reset mux ... for NONE polarity doesn't matter */
	board_set_usb_mux(port, TYPEC_MUX_NONE, USB_SWITCH_DISCONNECT, 0);

	if (port == 0) {
			gpio_set_level(GPIO_USB_C0_CC1_VCONN1_EN_L, 1);
			gpio_set_level(GPIO_USB_C0_CC2_VCONN1_EN_L, 1);
			gpio_set_level(GPIO_USB_C0_DP_HPD, 0);
	} else {
			gpio_set_level(GPIO_USB_C1_CC1_VCONN1_EN_L, 1);
			gpio_set_level(GPIO_USB_C1_CC2_VCONN1_EN_L, 1);
			gpio_set_level(GPIO_USB_C1_DP_HPD, 0);
	}
}

static inline int pd_adc_read(int port, int cc)
{
	if (port == 0)
		return adc_read_channel(cc ? ADC_C0_CC2_PD : ADC_C0_CC1_PD);
	else
		return adc_read_channel(cc ? ADC_C1_CC2_PD : ADC_C1_CC1_PD);
}

static inline void pd_set_vconn(int port, int polarity, int enable)
{
	/* Set VCONN on the opposite CC line from the polarity */
	if (port == 0)
		gpio_set_level(polarity ? GPIO_USB_C0_CC1_VCONN1_EN_L :
					  GPIO_USB_C0_CC2_VCONN1_EN_L, !enable);
	else
		gpio_set_level(polarity ? GPIO_USB_C1_CC1_VCONN1_EN_L :
					  GPIO_USB_C1_CC2_VCONN1_EN_L, !enable);
}

static inline int pd_snk_is_vbus_provided(int port)
{
	return gpio_get_level(port ? GPIO_USB_C1_VBUS_WAKE :
				     GPIO_USB_C0_VBUS_WAKE);
}

/* Standard-current DFP : no-connect voltage is 1.55V */
#define PD_SRC_VNC 1550 /* mV */

/* UFP-side : threshold for DFP connection detection */
#define PD_SNK_VA   200 /* mV */

/* start as a sink in case we have no other power supply/battery */
#define PD_DEFAULT_STATE PD_STATE_SNK_DISCONNECTED

/*
 * delay to turn on the power supply max is ~16ms.
 * delay to turn off the power supply max is about ~180ms.
 */
#define PD_POWER_SUPPLY_TURN_ON_DELAY  30000  /* us */
#define PD_POWER_SUPPLY_TURN_OFF_DELAY 250000 /* us */

/* delay to turn on/off vconn */
#define PD_VCONN_SWAP_DELAY 5000 /* us */

/* Define typical operating power and max power */
#define PD_OPERATING_POWER_MW 15000
#define PD_MAX_POWER_MW       60000
#define PD_MAX_CURRENT_MA     3000
#define PD_MAX_VOLTAGE_MV     20000

#endif /* __USB_PD_CONFIG_H */
