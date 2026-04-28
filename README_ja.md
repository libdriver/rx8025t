[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver RX8025T

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/rx8025t/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

このモジュールは、32.768kHzのDTCXOを内蔵したI2Cバスインターフェース準拠のリアルタイムクロックです。カレンダー機能（年、月、日、曜日、時、分、秒）とクロックカウンタ機能に加え、アラーム機能、固定サイクルタイマー機能、時刻更新割り込み機能、32.768kHz出力機能など、豊富な機能を備えています。このモジュールに搭載されているデバイスは、低消費電流を実現するCMOSプロセスで製造されており、長時間のバッテリーバックアップが可能です。これらの多くの機能はすべてSOPパッケージに実装されています。

LibDriver RX8025Tは、LibDriver社がリリースしたRX8025T用のフル機能ドライバです。時刻読み取り、アラーム、タイマーなどの機能を提供します。LibDriverはMISRA規格に準拠しています。

### 目次

  - [説明](#説明)
  - [インストール](#インストール)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example alarm](#example-alarm)
    - [example timer](#example-timer)
  - [ドキュメント](#ドキュメント)
  - [貢献](#貢献)
  - [著作権](#著作権)
  - [連絡して](#連絡して)

### 説明

/ srcディレクトリには、LibDriver RX8025Tのソースファイルが含まれています。

/ interfaceディレクトリには、LibDriver RX8025T用のプラットフォームに依存しないIICバステンプレートが含まれています。

/ testディレクトリには、チップの必要な機能を簡単にテストできるLibDriver RX8025Tドライバーテストプログラムが含まれています。

/ exampleディレクトリには、LibDriver RX8025Tプログラミング例が含まれています。

/ docディレクトリには、LibDriver RX8025Tオフラインドキュメントが含まれています。

/ datasheetディレクトリには、RX8025Tデータシートが含まれています。

/ projectディレクトリには、一般的に使用されるLinuxおよびマイクロコントローラー開発ボードのプロジェクトサンプルが含まれています。 すべてのプロジェクトは、デバッグ方法としてシェルスクリプトを使用しています。詳細については、各プロジェクトのREADME.mdを参照してください。

/ misraはLibDriver misraコードスキャン結果を含む。

### インストール

/ interfaceディレクトリにあるプラットフォームに依存しないIICバステンプレートを参照して、指定したプラットフォームのIICバスドライバを完成させます。

/src ディレクトリ、プラットフォームのインターフェイス ドライバー、および独自のドライバーをプロジェクトに追加します。デフォルトのサンプル ドライバーを使用する場合は、/example ディレクトリをプロジェクトに追加します。

### 使用

/example ディレクトリ内のサンプルを参照して、独自のドライバーを完成させることができます。 デフォルトのプログラミング例を使用したい場合の使用方法は次のとおりです。

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

```c
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

```c
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

### ドキュメント

オンラインドキュメント: [https://www.libdriver.com/docs/rx8025t/index.html](https://www.libdriver.com/docs/rx8025t/index.html)。

オフラインドキュメント: /doc/html/index.html。

### 貢献

CONTRIBUTING.mdを参照してください。

### 著作権

著作権（c）2015-今 LibDriver 全著作権所有

MITライセンス（MIT）

このソフトウェアおよび関連するドキュメントファイル（「ソフトウェア」）のコピーを取得した人は、無制限の使用、複製、変更、組み込み、公開、配布、サブライセンスを含む、ソフトウェアを処分する権利を制限なく付与されます。ソフトウェアのライセンスおよび/またはコピーの販売、および上記のようにソフトウェアが配布された人の権利のサブライセンスは、次の条件に従うものとします。

上記の著作権表示およびこの許可通知は、このソフトウェアのすべてのコピーまたは実体に含まれるものとします。

このソフトウェアは「現状有姿」で提供され、商品性、特定目的への適合性、および非侵害の保証を含むがこれらに限定されない、明示または黙示を問わず、いかなる種類の保証もありません。 いかなる場合も、作者または著作権所有者は、契約、不法行為、またはその他の方法で、本ソフトウェアおよび本ソフトウェアの使用またはその他の廃棄に起因または関連して、請求、損害、またはその他の責任を負わないものとします。

### 連絡して

お問い合わせくださいlishifenging@outlook.com。