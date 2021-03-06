/* -*- mode:c -*-
 *
 * Copyright (c) 2014 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

/* Inputs with interrupt handlers are first for efficiency */
GPIO_INT(POWER_BUTTON_L, B, 5,  GPIO_INT_BOTH,                power_button_interrupt) /* wk6 */ /* active high, the name is for compatibility with existing code */
GPIO_INT(SOC_POWER_GOOD, A, 3,  GPIO_INT_BOTH,                power_signal_interrupt)
GPIO_INT(LID_OPEN,       C, 13, GPIO_INT_BOTH,                lid_interrupt)
GPIO_INT(SUSPEND_L,      C, 7,  GPIO_INT_BOTH,                power_signal_interrupt)
GPIO_INT(SPI1_NSS,       A, 4,  GPIO_INT_BOTH,                spi_event)
GPIO_INT(AC_PRESENT,     A, 8,  GPIO_INT_BOTH | GPIO_PULL_UP, extpower_interrupt)

/* Keyboard inputs */
GPIO_INT(KB_IN00, C, 8,  GPIO_KB_INPUT, keyboard_raw_gpio_interrupt)
GPIO_INT(KB_IN01, C, 9,  GPIO_KB_INPUT, keyboard_raw_gpio_interrupt)
GPIO_INT(KB_IN02, C, 10, GPIO_KB_INPUT, keyboard_raw_gpio_interrupt)
GPIO_INT(KB_IN03, C, 11, GPIO_KB_INPUT, keyboard_raw_gpio_interrupt)
GPIO_INT(KB_IN04, C, 12, GPIO_KB_INPUT, keyboard_raw_gpio_interrupt)
GPIO_INT(KB_IN05, C, 14, GPIO_KB_INPUT, keyboard_raw_gpio_interrupt)
GPIO_INT(KB_IN06, C, 15, GPIO_KB_INPUT, keyboard_raw_gpio_interrupt)
GPIO_INT(KB_IN07, D, 2,  GPIO_KB_INPUT, keyboard_raw_gpio_interrupt)

/* Other inputs */
GPIO(EC_WAKE, A, 0, GPIO_INPUT | GPIO_PULL_DOWN) /* wk1 */
GPIO(WP_L,    B, 4, GPIO_INPUT)

/* Outputs */
GPIO(BAT_LED0,          B, 11, GPIO_OUT_LOW)
GPIO(BAT_LED1,          A, 11, GPIO_OUT_LOW)
GPIO(EC_BL_OVERRIDE,    F, 1,  GPIO_OUT_LOW)
GPIO(EC_INT,            B, 9,  GPIO_OUT_LOW)
GPIO(ENTERING_RW,       F, 0,  GPIO_OUT_LOW)
GPIO(I2C1_SCL,          B, 6,  GPIO_ODR_HIGH)
GPIO(I2C1_SDA,          B, 7,  GPIO_ODR_HIGH)
GPIO(KB_OUT00,          B, 0,  GPIO_KB_OUTPUT)
GPIO(KB_OUT01,          B, 8,  GPIO_KB_OUTPUT)
GPIO(KB_OUT02,          B, 12, GPIO_OUT_LOW) /* Inverted from silegro */
GPIO(KB_OUT03,          B, 13, GPIO_KB_OUTPUT)
GPIO(KB_OUT04,          B, 14, GPIO_KB_OUTPUT)
GPIO(KB_OUT05,          B, 15, GPIO_KB_OUTPUT)
GPIO(KB_OUT06,          C, 0,  GPIO_KB_OUTPUT)
GPIO(KB_OUT07,          C, 1,  GPIO_KB_OUTPUT)
GPIO(KB_OUT08,          C, 2,  GPIO_KB_OUTPUT)
GPIO(KB_OUT09,          B, 1,  GPIO_KB_OUTPUT)
GPIO(KB_OUT10,          C, 5,  GPIO_KB_OUTPUT)
GPIO(KB_OUT11,          C, 4,  GPIO_KB_OUTPUT)
GPIO(KB_OUT12,          A, 13, GPIO_KB_OUTPUT)
GPIO(POWER_LED,         A, 2,  GPIO_OUT_LOW)
GPIO(PMIC_PWRON,        A, 12, GPIO_OUT_LOW)
GPIO(PMIC_RESET,        B, 3,  GPIO_OUT_LOW)
GPIO(PMIC_SOURCE_PWREN, B, 10, GPIO_OUT_LOW)
GPIO(PMIC_WARM_RESET_L, C, 3,  GPIO_ODR_HIGH)

ALTERNATE(A, 0x00f0, 0, MODULE_SPI,  0)
ALTERNATE(A, 0x0600, 1, MODULE_UART, 0)
ALTERNATE(B, 0x00c0, 1, MODULE_I2C,  0)
