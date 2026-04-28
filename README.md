[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver RX8025T

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/rx8025t/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

This module is an I2C bus interface-compliant real-time clock which includes a 32.768 kHz DTCXO. In addition to providing a calendar (year, month, date, day, hour, minute, second) function and a clock counter function, this module provides an abundance of other functions including an alarm function, fixed-cycle timer function, time update interrupt function, and 32.768 kHz output function. The devices in this module are fabricated via a C-MOS process for low current consumption, which enables long-term battery back-up. All of these many functions are implemented in a SOP package.

LibDriver RX8025T is a full-featured driver for RX8025T, launched by LibDriver. It provides time reading, alarm, timer and additional features. LibDriver is MISRA compliant.

### Table of Contents

  - [Instruction](#Instruction)
  - [Install](#Install)
  - [Usage](#Usage)
    - [example basic](#example-basic)
    - [example alarm](#example-alarm)
    - [example timer](#example-timer)
  - [Document](#Document)
  - [Contributing](#Contributing)
  - [License](#License)
  - [Contact Us](#Contact-Us)

### Instruction

/src includes LibDriver RX8025T source files.

/interface includes LibDriver RX8025T IIC platform independent template.

/test includes LibDriver RX8025T driver test code and this code can test the chip necessary function simply.

/example includes LibDriver RX8025T sample code.

/doc includes LibDriver RX8025T offline document.

/datasheet includes RX8025T datasheet.

/project includes the common Linux and MCU development board sample code. All projects use the shell script to debug the driver and the detail instruction can be found in each project's README.md.

/misra includes the LibDriver MISRA code scanning results.

### Install

Reference /interface IIC platform independent template and finish your platform IIC driver.

Add the /src directory, the interface driver for your platform, and your own drivers to your project, if you want to use the default example drivers, add the /example directory to your project.

### Usage

You can refer to the examples in the /example directory to complete your own driver. If you want to use the default programming examples, here's how to use them.

#### example basic

```C
#include "driver_rx8025t_basic.h"

uint8_t res;
uint8_t data;
rx8025t_time_t t;
uint32_t timestamp;

/* basic init */
res = rx8025t_basic_init();
if (res != 0)
{
    return 1;
}

...
    
/* set the zone */
res = rx8025t_basic_set_timestamp_time_zone(8);
if (res != 0)
{
    (void)rx8025t_basic_deinit();

    return 1;
}

/* set time */
res = rx8025t_basic_set_timestamp((time_t)timestamp);
if (res != 0)
{
    (void)rx8025t_basic_deinit();

    return 1;
}

/* output */
rx8025t_interface_debug_print("rx8025t: set timestamp %d.\n", (time_t)timestamp);

...
    
/* basic get time */
res = rx8025t_basic_get_time(&t);
if (res != 0)
{
    (void)rx8025t_basic_deinit();

    return 1;
}

/* output */
rx8025t_interface_debug_print("rx8025t: %04d-%02d-%02d %02d:%02d:%02d %d.\n",
                              t.year, t.month, t.date,
                              t.hour, t.minute, t.second, t.week
                             );

...

/* write ram */
res = rx8025t_basic_write_ram(data);
if (res != 0)
{
    (void)rx8025t_basic_deinit();

    return 1;
}

/* output */
rx8025t_interface_debug_print("rx8025t: ram write 0x%02X.\n", data);

...

/* read ram */
res = rx8025t_basic_read_ram(&data);
if (res != 0)
{
    (void)rx8025t_basic_deinit();

    return 1;
}

/* output */
rx8025t_interface_debug_print("rx8025t: ram read 0x%02X.\n", data);  

...
    
/* basic deinit */
(void)rx8025t_basic_deinit();

return 0;
```

#### example alarm

```C
#include "driver_rx8025t_alarm.h"

uint8_t res;
static uint8_t gs_flag;
uint8_t (*g_gpio_irq)(void) = NULL;
uint8_t date = 0x00;
uint8_t hour = 0x00;
uint8_t minute = 0x00;
uint8_t weekday = 0x01;
uint32_t timeout = 60;
rx8025t_bool_t date_weekday_mask = RX8025T_BOOL_TRUE;
rx8025t_bool_t hour_mask = RX8025T_BOOL_TRUE;
rx8025t_bool_t minute_mask = RX8025T_BOOL_TRUE;
rx8025t_alarm_t date_week = RX8025T_ALARM_DAY;

static void a_alarm_callback(uint8_t type)
{
    switch (type)
    {
        case RX8025T_STATUS_UPDATE :
        {
            break;
        }
        case RX8025T_STATUS_TIMER :
        {
            break;
        }
        case RX8025T_STATUS_ALARM :
        {
            rx8025t_interface_debug_print("rx8025t: irq alarm.\n");
            gs_flag = 1;
            
            break;
        }
        case RX8025T_STATUS_VOLTAGE_LOW :
        {
            rx8025t_interface_debug_print("rx8025t: irq voltage low.\n");
            
            break;
        }
        case RX8025T_STATUS_VOLTAGE_DETECTION :
        {
            rx8025t_interface_debug_print("rx8025t: irq voltage detection.\n");
            
            break;
        }
        default :
        {
            rx8025t_interface_debug_print("rx8025t: unknown code.\n");
            
            break;
        }
    }
}

/* alarm init */
res = rx8025t_alarm_init(a_alarm_callback);
if (res != 0)
{
    return 1;
}

...
    
/* alarm config */
res = rx8025t_alarm_config(minute, hour, date_week, date, weekday,
                           minute_mask, hour_mask, date_weekday_mask);
if (res != 0)
{
    (void)rx8025t_alarm_deinit();

    return 1;
}

/* output */
rx8025t_interface_debug_print("rx8025t: alarm minute is %d.\n", minute);
rx8025t_interface_debug_print("rx8025t: alarm hour is %d.\n", hour);
rx8025t_interface_debug_print("rx8025t: alarm date is %d.\n", date);
rx8025t_interface_debug_print("rx8025t: alarm weekday is %d.\n", weekday);
if (date_week == RX8025T_ALARM_DAY)
{
    rx8025t_interface_debug_print("rx8025t: alarm date mode.\n");
}
else
{
    rx8025t_interface_debug_print("rx8025t: alarm week mode.\n");
}
rx8025t_interface_debug_print("rx8025t: alarm minute mask is %s.\n", (minute_mask == RX8025T_BOOL_TRUE) ? "true" : "false");
rx8025t_interface_debug_print("rx8025t: alarm hour mask is %s.\n", (hour_mask == RX8025T_BOOL_TRUE) ? "true" : "false");
rx8025t_interface_debug_print("rx8025t: alarm date weekday mask is %s.\n", (date_weekday_mask == RX8025T_BOOL_TRUE) ? "true" : "false");

...
    
/* interrupt init */
if (gpio_interrupt_init() != 0)
{
    (void)rx8025t_alarm_deinit();

    return 1;
}
g_gpio_irq = rx8025t_alarm_irq_handler;

...
    
/* output */
rx8025t_interface_debug_print("rx8025t: start alarm.\n");

/* init 0 */
gs_flag = 0;

/* running */
while (timeout != 0)
{
    /* check flag */
    if (gs_flag != 0)
    {
        break;
    }

    /* delay 1s */
    rx8025t_interface_delay_ms(1000);
}

...
    
/* check timeout */
if (timeout != 0)
{
    /* output */
    rx8025t_interface_debug_print("rx8025t: end alarm.\n");
}
else
{
    /* output */
    rx8025t_interface_debug_print("rx8025t: timeout.\n");
}

...
    
/* alarm deinit */
(void)rx8025t_alarm_deinit();
(void)gpio_interrupt_deinit();
g_gpio_irq = NULL;

return 0;
```

#### example timer

```C
#include "driver_rx8025t_timer.h"

uint8_t res;
static uint8_t gs_flag;
uint8_t (*g_gpio_irq)(void) = NULL;
uint32_t t;
rx8025t_timer_clock_t clk = RX8025T_TIMER_CLOCK_4096_HZ;
uint16_t counter = 100;

static void a_timer_callback(uint8_t type)
{
    switch (type)
    {
        case RX8025T_STATUS_UPDATE :
        {
            break;
        }
        case RX8025T_STATUS_TIMER :
        {
            rx8025t_interface_debug_print("rx8025t: irq timer.\n");
            gs_flag = 1;
            
            break;
        }
        case RX8025T_STATUS_ALARM :
        {
            break;
        }
        case RX8025T_STATUS_VOLTAGE_LOW :
        {
            rx8025t_interface_debug_print("rx8025t: irq voltage low.\n");
            
            break;
        }
        case RX8025T_STATUS_VOLTAGE_DETECTION :
        {
            rx8025t_interface_debug_print("rx8025t: irq voltage detection.\n");
            
            break;
        }
        default :
        {
            rx8025t_interface_debug_print("rx8025t: unknown code.\n");
            
            break;
        }
    }
}

/* timer init */
res = rx8025t_timer_init(a_timer_callback);
if (res != 0)
{
    return 1;
}

...
    
/* timer config */
res = rx8025t_timer_config(clk, counter);
if (res != 0)
{
    (void)rx8025t_timer_deinit();

    return 1;
}

...
    
/* output */
if (clk == RX8025T_TIMER_CLOCK_4096_HZ)
{
    t = counter * 2 * 1;

    /* output */
    rx8025t_interface_debug_print("rx8025t: set clock 4096hz, counter %d.\n", counter);
}
else if (clk == RX8025T_TIMER_CLOCK_64_HZ)
{
    t = counter * 2 * 16;

    /* output */
    rx8025t_interface_debug_print("rx8025t: set clock 64hz, counter %d.\n", counter);
}
else if (clk == RX8025T_TIMER_CLOCK_1_HZ)
{
    t = counter * 2 * 1000;

    /* output */
    rx8025t_interface_debug_print("rx8025t: set clock 1hz, counter %d.\n", counter);
}
else
{
    t = counter * 2 * 1000 * 60;

    /* output */
    rx8025t_interface_debug_print("rx8025t: set clock 1min, counter %d.\n", counter);
}

...
    
/* interrupt init */
if (gpio_interrupt_init() != 0)
{
    (void)rx8025t_timer_deinit();

    return 1;
}
g_gpio_irq = rx8025t_timer_irq_handler;

...
    
/* output */
rx8025t_interface_debug_print("rx8025t: start timer.\n");

/* start */
res = rx8025t_timer_start();
if (res != 0)
{
    (void)rx8025t_timer_deinit();
    (void)gpio_interrupt_deinit();
    g_gpio_irq = NULL;

    return 1;
}

...
    
/* init 0 */
gs_flag = 0;

while (t != 0)
{
    /* check flag */
    if (gs_flag != 0)
    {
        break;
    }

    /* delay 1ms */
    rx8025t_interface_delay_ms(1);

    /* t-- */
    t--;
}

if (t != 0)
{
    /* output */
    rx8025t_interface_debug_print("rx8025t: end timer.\n");
}
else
{
    /* output */
    rx8025t_interface_debug_print("rx8025t: timeout.\n");
}

...
    
/* timer deinit */
(void)rx8025t_timer_stop();
(void)rx8025t_timer_deinit();
(void)gpio_interrupt_deinit();
g_gpio_irq = NULL;

return 0;
```

### Document

Online documents: [https://www.libdriver.com/docs/rx8025t/index.html](https://www.libdriver.com/docs/rx8025t/index.html).

Offline documents: /doc/html/index.html.

### Contributing

Please refer to CONTRIBUTING.md.

### License

Copyright (c) 2015 - present LibDriver All rights reserved



The MIT License (MIT) 



Permission is hereby granted, free of charge, to any person obtaining a copy

of this software and associated documentation files (the "Software"), to deal

in the Software without restriction, including without limitation the rights

to use, copy, modify, merge, publish, distribute, sublicense, and/or sell

copies of the Software, and to permit persons to whom the Software is

furnished to do so, subject to the following conditions: 



The above copyright notice and this permission notice shall be included in all

copies or substantial portions of the Software. 



THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR

IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,

FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE

AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER

LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,

OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE

SOFTWARE. 

### Contact Us

Please send an e-mail to lishifenging@outlook.com.