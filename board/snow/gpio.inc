/* -*- mode:c -*-
 *
 * Copyright (c) 2014 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

/* Inputs with interrupt handlers are first for efficiency */
GPIO_INT(KB_PWR_ON_L,    B, 5,  GPIO_INT_BOTH,     power_signal_interrupt) /* Keyboard power button */
GPIO_INT(PP1800_LDO2,    A, 1,  GPIO_INT_BOTH,     power_signal_interrupt) /* LDO2 is ON (end of PMIC sequence) */
GPIO_INT(SOC1V8_XPSHOLD, A, 3,  GPIO_INT_BOTH,     power_signal_interrupt) /* App Processor ON  */
GPIO_INT(CHARGER_INT_L,  C, 4,  GPIO_INT_FALLING,  pmu_irq_handler)
GPIO_INT(LID_OPEN,       C, 13, GPIO_INT_BOTH,     lid_interrupt)          /* LID switch detection */
GPIO_INT(SUSPEND_L,      A, 7,  INT_BOTH_FLOATING, power_signal_interrupt) /* AP suspend/resume state */
GPIO_INT(SPI1_NSS,       A, 4,  GPIO_DEFAULT,      spi_event)

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
GPIO(AC_PWRBTN_L, A, 0, GPIO_INT_BOTH)
GPIO(WP_L,        B, 4, GPIO_INPUT)       /* Write protection pin (low active) */

/*
 * I2C pins should be configured as inputs until I2C module is
 * initialized. This will avoid driving the lines unintentionally.
 */
GPIO(I2C1_SCL, B, 6,  GPIO_INPUT)
GPIO(I2C1_SDA, B, 7,  GPIO_INPUT)
GPIO(I2C2_SCL, B, 10, GPIO_INPUT)
GPIO(I2C2_SDA, B, 11, GPIO_INPUT)

/* Outputs */
GPIO(AC_STATUS,   A, 5,  GPIO_DEFAULT)
GPIO(SPI1_MISO,   A, 6,  GPIO_DEFAULT)
GPIO(EN_PP1350,   A, 2,  GPIO_OUT_LOW)    /* DDR 1.35v rail enable */
GPIO(EN_PP5000,   A, 11, GPIO_OUT_LOW)    /* 5.0v rail enable */
GPIO(EN_PP3300,   A, 8,  GPIO_OUT_LOW)    /* 3.3v rail enable */
GPIO(PMIC_PWRON_L,A, 12, GPIO_OUT_HIGH)   /* 5v rail ready */
GPIO(PMIC_RESET,  A, 15, GPIO_OUT_LOW)    /* Force hard reset of the pmic */

/* EC is R/W mode for the kbc mux */
GPIO(ENTERING_RW, D, 0,  GPIO_OUT_LOW)
GPIO(CHARGER_EN,  B, 2,  GPIO_OUT_LOW)
GPIO(EC_INT,      B, 9,  GPIO_ODR_HIGH)

/* To audio codec (KB noise cancellation) */
GPIO(CODEC_INT,   D, 1,  GPIO_ODR_HIGH)
GPIO(LED_POWER_L, B, 3,  GPIO_INPUT)      /* Keyboard power LED */
GPIO(KB_OUT00,    B, 0,  GPIO_KB_OUTPUT)
GPIO(KB_OUT01,    B, 8,  GPIO_KB_OUTPUT)
GPIO(KB_OUT02,    B, 12, GPIO_KB_OUTPUT)
GPIO(KB_OUT03,    B, 13, GPIO_KB_OUTPUT)
GPIO(KB_OUT04,    B, 14, GPIO_KB_OUTPUT)
GPIO(KB_OUT05,    B, 15, GPIO_KB_OUTPUT)
GPIO(KB_OUT06,    C, 0,  GPIO_KB_OUTPUT)
GPIO(KB_OUT07,    C, 1,  GPIO_KB_OUTPUT)
GPIO(KB_OUT08,    C, 2,  GPIO_KB_OUTPUT)
GPIO(KB_OUT09,    B, 1,  GPIO_KB_OUTPUT)
GPIO(KB_OUT10,    C, 5,  GPIO_KB_OUTPUT)
GPIO(KB_OUT11,    C, 6,  GPIO_KB_OUTPUT)
GPIO(KB_OUT12,    C, 7,  GPIO_KB_OUTPUT)
