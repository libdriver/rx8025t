[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver RX8025T

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/rx8025t/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE) 

Dieses Modul ist eine I2C-Bus-kompatible Echtzeituhr mit einem 32,768-kHz-DTCXO. Neben Kalenderfunktionen (Jahr, Monat, Datum, Wochentag, Stunde, Minute, Sekunde) und einem Zeitzähler bietet es zahlreiche weitere Funktionen, darunter eine Alarmfunktion, einen Timer mit festem Zyklus, eine Zeitaktualisierungs-Interruptfunktion und eine 32,768-kHz-Ausgabefunktion. Die Bauteile dieses Moduls werden im CMOS-Verfahren gefertigt, was einen geringen Stromverbrauch und somit eine langfristige Batterie-Backup-Funktion ermöglicht. Alle diese Funktionen sind in einem SOP-Gehäuse realisiert.

LibDriver RX8025T ist ein umfassender Treiber für den RX8025T, entwickelt von LibDriver. Er bietet Funktionen wie Zeitanzeige, Alarm, Timer und mehr. LibDriver ist MISRA-konform.

### Inhaltsverzeichnis

  - [Anweisung](#Anweisung)
  - [Installieren](#Installieren)
  - [Nutzung](#Nutzung)
    - [example basic](#example-basic)
    - [example alarm](#example-alarm)
    - [example timer](#example-timer)
  - [Dokument](#Dokument)
  - [Beitrag](#Beitrag)
  - [Lizenz](#Lizenz)
  - [Kontaktieren Sie uns](#Kontaktieren-Sie-uns)

### Anweisung

/src enthält LibDriver RX8025T-Quelldateien.

/interface enthält die plattformunabhängige Vorlage LibDriver RX8025T IIC.

/test enthält den Testcode des LibDriver RX8025T-Treibers und dieser Code kann die erforderliche Funktion des Chips einfach testen.

/example enthält LibDriver RX8025T-Beispielcode.

/doc enthält das LibDriver RX8025T-Offlinedokument.

/Datenblatt enthält RX8025T-Datenblatt.

/project enthält den allgemeinen Beispielcode für Linux- und MCU-Entwicklungsboards. Alle Projekte verwenden das Shell-Skript, um den Treiber zu debuggen, und die detaillierten Anweisungen finden Sie in der README.md jedes Projekts.

/misra enthält die Ergebnisse des LibDriver MISRA Code Scans.

### Installieren

Verweisen Sie auf eine plattformunabhängige IIC-Schnittstellenvorlage und stellen Sie Ihren Plattform-IIC-Treiber fertig.

Fügen Sie das Verzeichnis /src, den Schnittstellentreiber für Ihre Plattform und Ihre eigenen Treiber zu Ihrem Projekt hinzu. Wenn Sie die Standardbeispieltreiber verwenden möchten, fügen Sie das Verzeichnis /example zu Ihrem Projekt hinzu.

### Nutzung

Sie können auf die Beispiele im Verzeichnis /example zurückgreifen, um Ihren eigenen Treiber zu vervollständigen. Wenn Sie die Standardprogrammierbeispiele verwenden möchten, erfahren Sie hier, wie Sie diese verwenden.

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

### Dokument

Online-Dokumente: [https://www.libdriver.com/docs/rx8025t/index.html](https://www.libdriver.com/docs/rx8025t/index.html).

Offline-Dokumente: /doc/html/index.html.

### Beitrag

Bitte beachten Sie CONTRIBUTING.md.

### Lizenz

Urheberrechte © (c) 2015 - Gegenwart LibDriver Alle Rechte vorbehalten



Die MIT-Lizenz (MIT)



Hiermit wird jeder Person kostenlos die Erlaubnis erteilt, eine Kopie zu erhalten

dieser Software und zugehörigen Dokumentationsdateien (die „Software“) zu behandeln

in der Software ohne Einschränkung, einschließlich, aber nicht beschränkt auf die Rechte

zu verwenden, zu kopieren, zu modifizieren, zusammenzuführen, zu veröffentlichen, zu verteilen, unterzulizenzieren und/oder zu verkaufen

Kopien der Software und Personen, denen die Software gehört, zu gestatten

dazu eingerichtet werden, unter folgenden Bedingungen:



Der obige Urheberrechtshinweis und dieser Genehmigungshinweis müssen in allen enthalten sein

Kopien oder wesentliche Teile der Software.



DIE SOFTWARE WIRD "WIE BESEHEN" BEREITGESTELLT, OHNE JEGLICHE GEWÄHRLEISTUNG, AUSDRÜCKLICH ODER

STILLSCHWEIGEND, EINSCHLIESSLICH, ABER NICHT BESCHRÄNKT AUF DIE GEWÄHRLEISTUNG DER MARKTGÄNGIGKEIT,

EIGNUNG FÜR EINEN BESTIMMTEN ZWECK UND NICHTVERLETZUNG VON RECHTEN DRITTER. IN KEINEM FALL DARF DAS

AUTOREN ODER URHEBERRECHTSINHABER HAFTEN FÜR JEGLICHE ANSPRÜCHE, SCHÄDEN ODER ANDERE

HAFTUNG, OB AUS VERTRAG, DELIKT ODER ANDERWEITIG, ENTSTEHEND AUS,

AUS ODER IM ZUSAMMENHANG MIT DER SOFTWARE ODER DER VERWENDUNG ODER ANDEREN HANDLUNGEN MIT DER

SOFTWARE.

### Kontaktieren Sie uns

Bitte senden Sie eine E-Mail an lishifenging@outlook.com.