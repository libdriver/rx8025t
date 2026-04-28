[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver RX8025T

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/rx8025t/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

本模块是一个符合I2C总线接口的实时时钟，内置32.768 kHz DTCXO。除了提供日历（年、月、日期、星期、小时、分钟、秒）功能和时钟计数器功能外，该模块还提供了丰富的其他功能，包括报警功能、固定周期定时器功能、时间更新中断功能和32.768 kHz输出功能。该模块中的器件采用C-MOS工艺制造，具有低电流消耗特性，可实现长期电池备份。所有这些功能均集成在SOP封装中。

LibDriver RX8025T是LibDriver推出的RX8025T全功能驱动，该驱动提供时间读取、闹钟和定时器等功能并且它符合MISRA标准。

### 目录

  - [说明](#说明)
  - [安装](#安装)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example alarm](#example-alarm)
    - [example timer](#example-timer)
  - [文档](#文档)
  - [贡献](#贡献)
  - [版权](#版权)
  - [联系我们](#联系我们)

### 说明

/src目录包含了LibDriver RX8025T的源文件。

/interface目录包含了LibDriver RX8025T与平台无关的IIC总线模板。

/test目录包含了LibDriver RX8025T驱动测试程序，该程序可以简单的测试芯片必要功能。

/example目录包含了LibDriver RX8025T编程范例。

/doc目录包含了LibDriver RX8025T离线文档。

/datasheet目录包含了RX8025T数据手册。

/project目录包含了常用Linux与单片机开发板的工程样例。所有工程均采用shell脚本作为调试方法，详细内容可参考每个工程里面的README.md。

/misra目录包含了LibDriver MISRA代码扫描结果。

### 安装

参考/interface目录下与平台无关的IIC总线模板，完成指定平台的IIC总线驱动。

将/src目录，您使用平台的接口驱动和您开发的驱动加入工程，如果您想要使用默认的范例驱动，可以将/example目录加入您的工程。

### 使用

您可以参考/example目录下的编程范例完成适合您的驱动，如果您想要使用默认的编程范例，以下是它们的使用方法。

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

### 文档

在线文档: [https://www.libdriver.com/docs/rx8025t/index.html](https://www.libdriver.com/docs/rx8025t/index.html)。

离线文档: /doc/html/index.html。

### 贡献

请参考CONTRIBUTING.md。

### 版权

版权 (c) 2015 - 现在 LibDriver 版权所有

MIT 许可证（MIT）

特此免费授予任何获得本软件副本和相关文档文件（下称“软件”）的人不受限制地处置该软件的权利，包括不受限制地使用、复制、修改、合并、发布、分发、转授许可和/或出售该软件副本，以及再授权被配发了本软件的人如上的权利，须在下列条件下：

上述版权声明和本许可声明应包含在该软件的所有副本或实质成分中。

本软件是“如此”提供的，没有任何形式的明示或暗示的保证，包括但不限于对适销性、特定用途的适用性和不侵权的保证。在任何情况下，作者或版权持有人都不对任何索赔、损害或其他责任负责，无论这些追责来自合同、侵权或其它行为中，还是产生于、源于或有关于本软件以及本软件的使用或其它处置。

### 联系我们

请联系lishifenging@outlook.com。