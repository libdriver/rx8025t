[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver RX8025T

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/rx8025t/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

이 모듈은 32.768kHz DTCXO를 포함하는 I2C 버스 인터페이스 호환 실시간 시계입니다. 달력(년, 월, 일, 요일, 시, 분, 초) 및 시계 카운터 기능 외에도 알람 기능, 고정 주기 타이머 기능, 시간 업데이트 인터럽트 기능, 32.768kHz 출력 기능 등 다양한 기능을 제공합니다. 이 모듈의 소자는 저전력 소모를 위해 C-MOS 공정으로 제작되어 장기간 배터리 백업이 가능합니다. 이 모든 기능은 SOP 패키지에 구현되어 있습니다.

LibDriver RX8025T는 LibDriver에서 출시한 RX8025T용 완벽한 기능을 갖춘 드라이버입니다. 시간 읽기, 알람, 타이머 및 기타 여러 기능을 제공합니다. LibDriver는 MISRA 규정을 준수합니다.

### 콘텐츠

  - [설명](#설명)
  - [설치](#설치)
  - [사용](#사용)
    - [example basic](#example-basic)
    - [example alarm](#example-alarm)
    - [example timer](#example-timer)
  - [문서](#문서)
  - [기고](#기고)
  - [저작권](#저작권)
  - [문의하기](#문의하기)

### 설명

/src 디렉토리에는 LibDriver RX8025T의 소스 파일이 포함되어 있습니다.

/interface 디렉토리에는 LibDriver RX8025T용 플랫폼 독립적인 IIC버스 템플릿이 포함되어 있습니다.

/test 디렉토리에는 LibDriver RX8025T드라이버 테스트 프로그램이 포함되어 있어 칩의 필요한 기능을 간단히 테스트할 수 있습니다.

/example 디렉토리에는 LibDriver RX8025T프로그래밍 예제가 포함되어 있습니다.

/doc 디렉토리에는 LibDriver RX8025T오프라인 문서가 포함되어 있습니다.

/datasheet 디렉토리에는 RX8025T데이터시트가 있습니다.

/project 디렉토리에는 일반적으로 사용되는 Linux 및 마이크로컨트롤러 개발 보드의 프로젝트 샘플이 포함되어 있습니다. 모든 프로젝트는 디버깅 방법으로 셸 스크립트를 사용하며, 자세한 내용은 각 프로젝트의 README.md를 참조하십시오.

/misra 에는 LibDriver misra 코드 검색 결과가 포함됩니다.

### 설치

/interface 디렉토리에서 플랫폼 독립적인 IIC버스 템플릿을 참조하여 지정된 플랫폼에 대한 IIC버스 드라이버를 완성하십시오.

/src 디렉터리, 플랫폼용 인터페이스 드라이버 및 자체 드라이버를 프로젝트에 추가합니다. 기본 예제 드라이버를 사용하려면 /example 디렉터리를 프로젝트에 추가합니다.

### 사용

/example 디렉터리의 예제를 참조하여 자신만의 드라이버를 완성할 수 있습니다. 기본 프로그래밍 예제를 사용하려는 경우 사용 방법은 다음과 같습니다.

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

### 문서

온라인 문서: [https://www.libdriver.com/docs/rx8025t/index.html](https://www.libdriver.com/docs/rx8025t/index.html).

오프라인 문서: /doc/html/index.html.

### 기고

CONTRIBUTING.md 를 참조하십시오.

### 저작권

저작권 (c) 2015 - 지금 LibDriver 판권 소유

MIT 라이선스(MIT)

이 소프트웨어 및 관련 문서 파일("소프트웨어")의 사본을 얻은 모든 사람은 이에 따라 무제한 사용, 복제, 수정, 통합, 출판, 배포, 2차 라이선스를 포함하여 소프트웨어를 처분할 수 있는 권리가 부여됩니다. 소프트웨어의 사본에 대한 라이선스 및/또는 판매, 그리고 소프트웨어가 위와 같이 배포된 사람의 권리에 대한 2차 라이선스는 다음 조건에 따릅니다.

위의 저작권 표시 및 이 허가 표시는 이 소프트웨어의 모든 사본 또는 내용에 포함됩니다.

이 소프트웨어는 상품성, 특정 목적에의 적합성 및 비침해에 대한 보증을 포함하되 이에 국한되지 않는 어떠한 종류의 명시적 또는 묵시적 보증 없이 "있는 그대로" 제공됩니다. 어떤 경우에도 저자 또는 저작권 소유자는 계약, 불법 행위 또는 기타 방식에 관계없이 소프트웨어 및 기타 소프트웨어 사용으로 인해 발생하거나 이와 관련하여 발생하는 청구, 손해 또는 기타 책임에 대해 책임을 지지 않습니다.

### 문의하기

연락주세요lishifenging@outlook.com.