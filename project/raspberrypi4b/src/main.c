/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      main.c
 * @brief     main source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2026-04-21
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2026/04/21  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "driver_rx8025t_register_test.h"
#include "driver_rx8025t_timer_test.h"
#include "driver_rx8025t_alarm_test.h"
#include "driver_rx8025t_basic.h"
#include "driver_rx8025t_timer.h"
#include "driver_rx8025t_alarm.h"
#include "gpio.h"
#include <getopt.h>
#include <stdlib.h>
#include <math.h>

/**
 * @brief global var definition
 */
static uint8_t gs_flag;                    /**< flag */
uint8_t (*g_gpio_irq)(void) = NULL;        /**< gpio irq */

/**
 * @brief     timer callback
 * @param[in] type irq type
 * @note      none
 */
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

/**
 * @brief     alarm callback
 * @param[in] type irq type
 * @note      none
 */
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

/**
 * @brief     rx8025t full function
 * @param[in] argc arg numbers
 * @param[in] **argv arg address
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 5 param is invalid
 * @note      none
 */
uint8_t rx8025t(uint8_t argc, char **argv)
{
    int c;
    int longindex = 0;
    const char short_options[] = "hipe:t:";
    const struct option long_options[] =
    {
        {"help", no_argument, NULL, 'h'},
        {"information", no_argument, NULL, 'i'},
        {"port", no_argument, NULL, 'p'},
        {"example", required_argument, NULL, 'e'},
        {"test", required_argument, NULL, 't'},
        {"clock", required_argument, NULL, 1},
        {"counter", required_argument, NULL, 2},
        {"data", required_argument, NULL, 3},
        {"date", required_argument, NULL, 4},
        {"date-week", required_argument, NULL, 5},
        {"date-weekday-mask", required_argument, NULL, 6},
        {"hour", required_argument, NULL, 7},
        {"hour-mask", required_argument, NULL, 8},
        {"minute", required_argument, NULL, 9},
        {"minute-mask", required_argument, NULL, 10},
        {"timestamp", required_argument, NULL, 11},
        {"weekday", required_argument, NULL, 12},
        {"timeout", required_argument, NULL, 13},
        {NULL, 0, NULL, 0},
    };
    char type[33] = "unknown";
    uint8_t data = 0x00;
    uint8_t date = 0x00;
    uint8_t hour = 0x00;
    uint8_t minute = 0x00;
    uint8_t weekday = 0x01;
    uint8_t timestamp_flag = 0x00;
    uint16_t counter = 100;
    uint32_t timestamp = 0x00;
    uint32_t timeout = 60;
    rx8025t_bool_t date_weekday_mask = RX8025T_BOOL_TRUE;
    rx8025t_bool_t hour_mask = RX8025T_BOOL_TRUE;
    rx8025t_bool_t minute_mask = RX8025T_BOOL_TRUE;
    rx8025t_alarm_t date_week = RX8025T_ALARM_DAY;
    rx8025t_timer_clock_t clk = RX8025T_TIMER_CLOCK_4096_HZ;

    /* if no params */
    if (argc == 1)
    {
        /* goto the help */
        goto help;
    }

    /* init 0 */
    optind = 0;

    /* parse */
    do
    {
        /* parse the args */
        c = getopt_long(argc, argv, short_options, long_options, &longindex);

        /* judge the result */
        switch (c)
        {
            /* help */
            case 'h' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "h");

                break;
            }

            /* information */
            case 'i' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "i");

                break;
            }

            /* port */
            case 'p' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "p");

                break;
            }

            /* example */
            case 'e' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "e_%s", optarg);

                break;
            }

            /* test */
            case 't' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "t_%s", optarg);

                break;
            }

            /* clock */
            case 1 :
            {
                if (strcmp("4096HZ", optarg) == 0)
                {
                    clk = RX8025T_TIMER_CLOCK_4096_HZ;
                }
                else if (strcmp("64HZ", optarg) == 0)
                {
                    clk = RX8025T_TIMER_CLOCK_64_HZ;
                }
                else if (strcmp("1HZ", optarg) == 0)
                {
                    clk = RX8025T_TIMER_CLOCK_1_HZ;
                }
                else if (strcmp("MINUTE", optarg) == 0)
                {
                    clk = RX8025T_TIMER_CLOCK_MINUTE;
                }
                else
                {
                    return 5;
                }

                break;
            }
            
            /* counter */
            case 2 :
            {
                counter = atol(optarg) & 0xFFFF;
                
                break;
            }
            
            /* data */
            case 3 :
            {
                char *p;
                uint16_t l;
                uint16_t i;
                uint64_t hex_data;

                /* set the data */
                l = strlen(optarg);

                /* check the header */
                if (l >= 2)
                {
                    if (strncmp(optarg, "0x", 2) == 0)
                    {
                        p = optarg + 2;
                        l -= 2;
                    }
                    else if (strncmp(optarg, "0X", 2) == 0)
                    {
                        p = optarg + 2;
                        l -= 2;
                    }
                    else
                    {
                        p = optarg;
                    }
                }
                else
                {
                    p = optarg;
                }

                /* init 0 */
                hex_data = 0;

                /* loop */
                for (i = 0; i < l; i++)
                {
                    if ((p[i] <= '9') && (p[i] >= '0'))
                    {
                        hex_data += (p[i] - '0') * (uint32_t)pow(16, l - i - 1);
                    }
                    else if ((p[i] <= 'F') && (p[i] >= 'A'))
                    {
                        hex_data += ((p[i] - 'A') + 10) * (uint32_t)pow(16, l - i - 1);
                    }
                    else if ((p[i] <= 'f') && (p[i] >= 'a'))
                    {
                        hex_data += ((p[i] - 'a') + 10) * (uint32_t)pow(16, l - i - 1);
                    }
                    else
                    {
                        return 5;
                    }
                }

                /* set the data */
                data = hex_data % 0xFF;

                break;
            }

            /* date */
            case 4 :
            {
                date = atol(optarg) & 0xFF;
                
                break;
            }

            /* date week */
            case 5 :
            {
                /* set the mode */
                if (strcmp("DATE", optarg) == 0)
                {
                    date_week = RX8025T_ALARM_DAY;
                }
                else if (strcmp("WEEK", optarg) == 0)
                {
                    date_week = RX8025T_ALARM_WEEK;
                }
                else
                {
                    return 5;
                }

                break;
            }

            /* date weekday mask */
            case 6 :
            {
                /* set the bool */
                if (strcmp("true", optarg) == 0)
                {
                    date_weekday_mask = RX8025T_BOOL_TRUE;
                }
                else if (strcmp("false", optarg) == 0)
                {
                    date_weekday_mask = RX8025T_BOOL_FALSE;
                }
                else
                {
                    return 5;
                }

                break;
            }

            /* hour */
            case 7 :
            {
                hour = atol(optarg) & 0xFF;
                
                break;
            }
            
            /* hour mask */
            case 8 :
            {
                /* set the bool */
                if (strcmp("true", optarg) == 0)
                {
                    hour_mask = RX8025T_BOOL_TRUE;
                }
                else if (strcmp("false", optarg) == 0)
                {
                    hour_mask = RX8025T_BOOL_FALSE;
                }
                else
                {
                    return 5;
                }

                break;
            }
            
            /* minute */
            case 9 :
            {
                minute = atol(optarg) & 0xFF;
                
                break;
            }
            
            /* minute mask */
            case 10 :
            {
                /* set the bool */
                if (strcmp("true", optarg) == 0)
                {
                    minute_mask = RX8025T_BOOL_TRUE;
                }
                else if (strcmp("false", optarg) == 0)
                {
                    minute_mask = RX8025T_BOOL_FALSE;
                }
                else
                {
                    return 5;
                }

                break;
            }
            
            /* timestamp */
            case 11 :
            {
                timestamp = atol(optarg);
                timestamp_flag = 1;
                
                break;
            }
            
            /* weekday */
            case 12 :
            {
                weekday = atol(optarg) & 0xFF;
                
                break;
            }
            
            /* timeout */
            case 13 :
            {
                timeout = atol(optarg);
                
                break;
            }
            
            /* the end */
            case -1 :
            {
                break;
            }

            /* others */
            default :
            {
                return 5;
            }
        }
    } while (c != -1);

    /* run the function */
    if (strcmp("t_reg", type) == 0)
    {
        /* run reg test */
        if (rx8025t_register_test() != 0)
        {
            return 1;
        }
        
        return 0;
    }
    else if (strcmp("t_timer", type) == 0)
    {
        /* interrupt init */
        if (gpio_interrupt_init() != 0)
        {
            return 1;
        }
        g_gpio_irq = rx8025t_timer_test_irq_handler;
        
        /* run timer test */
        if (rx8025t_timer_test() != 0)
        {
            g_gpio_irq = NULL;
            (void)gpio_interrupt_deinit();
            
            return 1;
        }
        
        /* deinit */
        g_gpio_irq = NULL;
        (void)gpio_interrupt_deinit();
        
        return 0;
    }
    else if (strcmp("t_alarm", type) == 0)
    {
        /* interrupt init */
        if (gpio_interrupt_init() != 0)
        {
            return 1;
        }
        g_gpio_irq = rx8025t_alarm_test_irq_handler;
        
        /* run alarm test */
        if (rx8025t_alarm_test() != 0)
        {
            g_gpio_irq = NULL;
            (void)gpio_interrupt_deinit();
            
            return 1;
        }
        
        /* deinit */
        g_gpio_irq = NULL;
        (void)gpio_interrupt_deinit();
        
        return 0;
    }
    else if (strcmp("e_basic-set-time", type) == 0)
    {
        uint8_t res;
        
        /* check timestamp */
        if (timestamp_flag == 0)
        {
            rx8025t_interface_debug_print("rx8025t: no timestamp.\n");
            
            return 5;
        }
        
        /* basic init */
        res = rx8025t_basic_init();
        if (res != 0)
        {
            return 1;
        }

        /* set the zone */
        res = rx8025t_basic_set_timestamp_time_zone(0);
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

        /* basic deinit */
        (void)rx8025t_basic_deinit();

        return 0;
    }
    else if (strcmp("e_basic-get-time", type) == 0)
    {
        uint8_t res;
        rx8025t_time_t t;

        /* basic init */
        res = rx8025t_basic_init();
        if (res != 0)
        {
            return 1;
        }

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
        
        /* basic deinit */
        (void)rx8025t_basic_deinit();

        return 0;
    }
    else if (strcmp("e_basic-set-ram", type) == 0)
    {
        uint8_t res;
        
        /* basic init */
        res = rx8025t_basic_init();
        if (res != 0)
        {
            return 1;
        }

        /* write ram */
        res = rx8025t_basic_write_ram(data);
        if (res != 0)
        {
            (void)rx8025t_basic_deinit();

            return 1;
        }

        /* output */
        rx8025t_interface_debug_print("rx8025t: ram write 0x%02X.\n", data);

        /* basic deinit */
        (void)rx8025t_basic_deinit();

        return 0;
    }
    else if (strcmp("e_basic-get-ram", type) == 0)
    {
        uint8_t res;
        
        /* basic init */
        res = rx8025t_basic_init();
        if (res != 0)
        {
            return 1;
        }

        /* read ram */
        res = rx8025t_basic_read_ram(&data);
        if (res != 0)
        {
            (void)rx8025t_basic_deinit();

            return 1;
        }

        /* output */
        rx8025t_interface_debug_print("rx8025t: ram read 0x%02X.\n", data);

        /* basic deinit */
        (void)rx8025t_basic_deinit();

        return 0;
    }
    else if (strcmp("e_timer-set-time", type) == 0)
    {
        uint8_t res;
        
        /* check timestamp */
        if (timestamp_flag == 0)
        {
            rx8025t_interface_debug_print("rx8025t: no timestamp.\n");
            
            return 5;
        }
        
        /* timer init */
        res = rx8025t_timer_init(a_timer_callback);
        if (res != 0)
        {
            return 1;
        }

        /* set the zone */
        res = rx8025t_timer_set_timestamp_time_zone(0);
        if (res != 0)
        {
            (void)rx8025t_timer_deinit();

            return 1;
        }

        /* set time */
        res = rx8025t_timer_set_timestamp((time_t)timestamp);
        if (res != 0)
        {
            (void)rx8025t_timer_deinit();

            return 1;
        }

        /* output */
        rx8025t_interface_debug_print("rx8025t: set timestamp %d.\n", (time_t)timestamp);

        /* timer deinit */
        (void)rx8025t_timer_deinit();

        return 0;
    }
    else if (strcmp("e_timer-get-time", type) == 0)
    {
        uint8_t res;
        rx8025t_time_t t;

        /* timer init */
        res = rx8025t_timer_init(a_timer_callback);
        if (res != 0)
        {
            return 1;
        }

        /* timer get time */
        res = rx8025t_timer_get_time(&t);
        if (res != 0)
        {
            (void)rx8025t_timer_deinit();

            return 1;
        }

        /* output */
        rx8025t_interface_debug_print("rx8025t: %04d-%02d-%02d %02d:%02d:%02d %d.\n",
                                      t.year, t.month, t.date,
                                      t.hour, t.minute, t.second, t.week
                                     );
        
        /* timer deinit */
        (void)rx8025t_timer_deinit();

        return 0;
    }
    else if (strcmp("e_timer-set-ram", type) == 0)
    {
        uint8_t res;
        
        /* timer init */
        res = rx8025t_timer_init(a_timer_callback);
        if (res != 0)
        {
            return 1;
        }

        /* write ram */
        res = rx8025t_timer_write_ram(data);
        if (res != 0)
        {
            (void)rx8025t_timer_deinit();

            return 1;
        }

        /* output */
        rx8025t_interface_debug_print("rx8025t: ram write 0x%02X.\n", data);

        /* timer deinit */
        (void)rx8025t_timer_deinit();

        return 0;
    }
    else if (strcmp("e_timer-get-ram", type) == 0)
    {
        uint8_t res;
        
        /* timer init */
        res = rx8025t_timer_init(a_timer_callback);
        if (res != 0)
        {
            return 1;
        }

        /* read ram */
        res = rx8025t_timer_read_ram(&data);
        if (res != 0)
        {
            (void)rx8025t_timer_deinit();

            return 1;
        }

        /* output */
        rx8025t_interface_debug_print("rx8025t: ram read 0x%02X.\n", data);

        /* timer deinit */
        (void)rx8025t_timer_deinit();

        return 0;
    }
    else if (strcmp("e_timer-config", type) == 0)
    {
        uint8_t res;
        uint32_t t;
        
        /* timer init */
        res = rx8025t_timer_init(a_timer_callback);
        if (res != 0)
        {
            return 1;
        }
        
        /* timer config */
        res = rx8025t_timer_config(clk, counter);
        if (res != 0)
        {
            (void)rx8025t_timer_deinit();
            
            return 1;
        }
        
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
        
        /* interrupt init */
        if (gpio_interrupt_init() != 0)
        {
            (void)rx8025t_timer_deinit();
            
            return 1;
        }
        g_gpio_irq = rx8025t_timer_irq_handler;
        
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
        
        /* timer deinit */
        (void)rx8025t_timer_stop();
        (void)rx8025t_timer_deinit();
        (void)gpio_interrupt_deinit();
        g_gpio_irq = NULL;
        
        return 0;
    }
    else if (strcmp("e_alarm-set-time", type) == 0)
    {
        uint8_t res;
        
        /* check timestamp */
        if (timestamp_flag == 0)
        {
            rx8025t_interface_debug_print("rx8025t: no timestamp.\n");
            
            return 5;
        }
        
        /* alarm init */
        res = rx8025t_alarm_init(a_alarm_callback);
        if (res != 0)
        {
            return 1;
        }

        /* set the zone */
        res = rx8025t_alarm_set_timestamp_time_zone(0);
        if (res != 0)
        {
            (void)rx8025t_alarm_deinit();

            return 1;
        }

        /* set time */
        res = rx8025t_alarm_set_timestamp((time_t)timestamp);
        if (res != 0)
        {
            (void)rx8025t_alarm_deinit();

            return 1;
        }

        /* output */
        rx8025t_interface_debug_print("rx8025t: set timestamp %d.\n", (time_t)timestamp);

        /* alarm deinit */
        (void)rx8025t_alarm_deinit();

        return 0;
    }
    else if (strcmp("e_alarm-get-time", type) == 0)
    {
        uint8_t res;
        rx8025t_time_t t;

        /* alarm init */
        res = rx8025t_alarm_init(a_alarm_callback);
        if (res != 0)
        {
            return 1;
        }

        /* alarm get time */
        res = rx8025t_alarm_get_time(&t);
        if (res != 0)
        {
            (void)rx8025t_alarm_deinit();

            return 1;
        }

        /* output */
        rx8025t_interface_debug_print("rx8025t: %04d-%02d-%02d %02d:%02d:%02d %d.\n",
                                      t.year, t.month, t.date,
                                      t.hour, t.minute, t.second, t.week
                                     );
        
        /* alarm deinit */
        (void)rx8025t_alarm_deinit();

        return 0;
    }
    else if (strcmp("e_alarm-set-ram", type) == 0)
    {
        uint8_t res;
        
        /* alarm init */
        res = rx8025t_alarm_init(a_alarm_callback);
        if (res != 0)
        {
            return 1;
        }

        /* write ram */
        res = rx8025t_alarm_write_ram(data);
        if (res != 0)
        {
            (void)rx8025t_alarm_deinit();

            return 1;
        }

        /* output */
        rx8025t_interface_debug_print("rx8025t: ram write 0x%02X.\n", data);

        /* alarm deinit */
        (void)rx8025t_alarm_deinit();

        return 0;
    }
    else if (strcmp("e_alarm-get-ram", type) == 0)
    {
        uint8_t res;
        
        /* alarm init */
        res = rx8025t_alarm_init(a_alarm_callback);
        if (res != 0)
        {
            return 1;
        }

        /* read ram */
        res = rx8025t_alarm_read_ram(&data);
        if (res != 0)
        {
            (void)rx8025t_alarm_deinit();

            return 1;
        }

        /* output */
        rx8025t_interface_debug_print("rx8025t: ram read 0x%02X.\n", data);

        /* alarm deinit */
        (void)rx8025t_alarm_deinit();

        return 0;
    }
    else if (strcmp("e_alarm-config", type) == 0)
    {
        uint8_t res;
        
        /* alarm init */
        res = rx8025t_alarm_init(a_alarm_callback);
        if (res != 0)
        {
            return 1;
        }
        
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
        
        /* interrupt init */
        if (gpio_interrupt_init() != 0)
        {
            (void)rx8025t_alarm_deinit();
            
            return 1;
        }
        g_gpio_irq = rx8025t_alarm_irq_handler;
        
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
        
        /* alarm deinit */
        (void)rx8025t_alarm_deinit();
        (void)gpio_interrupt_deinit();
        g_gpio_irq = NULL;
        
        return 0;
    }
    else if (strcmp("h", type) == 0)
    {
        help:
        rx8025t_interface_debug_print("Usage:\n");
        rx8025t_interface_debug_print("  rx8025t (-i | --information)\n");
        rx8025t_interface_debug_print("  rx8025t (-h | --help)\n");
        rx8025t_interface_debug_print("  rx8025t (-p | --port)\n");
        rx8025t_interface_debug_print("  rx8025t (-t reg | --test=reg)\n");
        rx8025t_interface_debug_print("  rx8025t (-t timer | --test=timer)\n");
        rx8025t_interface_debug_print("  rx8025t (-t alarm | --test=alarm)\n");
        rx8025t_interface_debug_print("  rx8025t (-e basic-set-time | --example=basic-set-time) --timestamp=<time>\n");
        rx8025t_interface_debug_print("  rx8025t (-e basic-get-time | --example=basic-get-time)\n");
        rx8025t_interface_debug_print("  rx8025t (-e basic-set-ram | --example=basic-set-ram) --data=<hex>\n");
        rx8025t_interface_debug_print("  rx8025t (-e basic-get-ram | --example=basic-get-ram)\n");
        rx8025t_interface_debug_print("  rx8025t (-e timer-set-time | --example=timer-set-time) --timestamp=<time>\n");
        rx8025t_interface_debug_print("  rx8025t (-e timer-get-time | --example=timer-get-time)\n");
        rx8025t_interface_debug_print("  rx8025t (-e timer-set-ram | --example=timer-set-ram) --data=<hex>\n");
        rx8025t_interface_debug_print("  rx8025t (-e timer-get-ram | --example=timer-get-ram)\n");
        rx8025t_interface_debug_print("  rx8025t (-e timer-config | --example=timer-config) --clock=<4096HZ | 64HZ | 1HZ | MINUTE> --counter=<num>\n");
        rx8025t_interface_debug_print("  rx8025t (-e alarm-set-time | --example=alarm-set-time) --timestamp=<time>\n");
        rx8025t_interface_debug_print("  rx8025t (-e alarm-get-time | --example=alarm-get-time)\n");
        rx8025t_interface_debug_print("  rx8025t (-e alarm-set-ram | --example=alarm-set-ram) --data=<hex>\n");
        rx8025t_interface_debug_print("  rx8025t (-e alarm-get-ram | --example=alarm-get-ram)\n");
        rx8025t_interface_debug_print("  rx8025t (-e alarm-config | --example=alarm-config) --minute=<num> --hour=<num> --date=<num> --weekday=<num>\n");
        rx8025t_interface_debug_print("          --date-week=<DATE | WEEK> --minute-mask=<true | false> --hour-mask=<true | false> --date-weekday-mask=<true | false>\n");
        rx8025t_interface_debug_print("          --timeout=<second>\n");
        rx8025t_interface_debug_print("\n");
        rx8025t_interface_debug_print("Options:\n");
        rx8025t_interface_debug_print("      --clock=<4096HZ | 64HZ | 1HZ | MINUTE>\n");
        rx8025t_interface_debug_print("                                  Set the timer clock.([default: 4096HZ])\n");
        rx8025t_interface_debug_print("      --counter=<num>             Set the timer counter.([default: 100])\n");
        rx8025t_interface_debug_print("      --data=<hex>                Set ram data and it is hexadecimal.([default: 0x00])\n");
        rx8025t_interface_debug_print("      --date=<num>                Set the alarm date.([default: 0])\n");
        rx8025t_interface_debug_print("      --date-week=<DATE | WEEK>   Set the alarm date or weekday.([default: DATE])\n");
        rx8025t_interface_debug_print("      --date-weekday-mask=<true | false>\n");
        rx8025t_interface_debug_print("                                  Set the alarm date or weekday mask.([default: true])\n");
        rx8025t_interface_debug_print("  -e <basic-set-time | basic-get-time | basic-set-ram | basic-get-ram | timer-set-time | timer-get-time |\n");
        rx8025t_interface_debug_print("      timer-set-ram | timer-get-ram | timer-config | alarm-set-time | alarm-get-time |\n");
        rx8025t_interface_debug_print("      alarm-set-ram | alarm-get-ram | alarm-config>, --example=<basic-set-time | basic-get-time | basic-set-ram |\n");
        rx8025t_interface_debug_print("      basic-get-ram | timer-set-time | timer-get-time | timer-set-ram | timer-get-ram | timer-config | alarm-set-time |\n");
        rx8025t_interface_debug_print("      alarm-get-time | alarm-set-ram | alarm-get-ram | alarm-config>\n");
        rx8025t_interface_debug_print("                                  Run the driver example.\n");
        rx8025t_interface_debug_print("  -h, --help                      Show the help.\n");
        rx8025t_interface_debug_print("      --hour=<num>                Set the alarm hour.([default: 0])\n");
        rx8025t_interface_debug_print("      --hour-mask=<true | false>  Set the alarm hour mask.([default: true])\n");
        rx8025t_interface_debug_print("  -i, --information               Show the chip information.\n");
        rx8025t_interface_debug_print("      --minute=<num>              Set the alarm minute.([default: 0])\n");
        rx8025t_interface_debug_print("      --minute-mask=<true | false>\n");
        rx8025t_interface_debug_print("                                  Set the alarm minute mask.([default: true])\n");
        rx8025t_interface_debug_print("  -p, --port                      Display the pin connections of the current board.\n");
        rx8025t_interface_debug_print("  -t <reg | timer | alarm>, --test=<reg | timer | alarm>\n");
        rx8025t_interface_debug_print("                                  Run the driver test.\n");
        rx8025t_interface_debug_print("      --timestamp=<time>          Set the the unix timestamp.\n");
        rx8025t_interface_debug_print("      --weekday=<num>             Set the alarm weekday.([default: 1])\n");
        rx8025t_interface_debug_print("      --timeout=<second>          Set the timeout in second.([default: 60])\n");

        return 0;
    }
    else if (strcmp("i", type) == 0)
    {
        rx8025t_info_t info;

        /* print rx8025t info */
        rx8025t_info(&info);
        rx8025t_interface_debug_print("rx8025t: chip is %s.\n", info.chip_name);
        rx8025t_interface_debug_print("rx8025t: manufacturer is %s.\n", info.manufacturer_name);
        rx8025t_interface_debug_print("rx8025t: interface is %s.\n", info.interface);
        rx8025t_interface_debug_print("rx8025t: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        rx8025t_interface_debug_print("rx8025t: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        rx8025t_interface_debug_print("rx8025t: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        rx8025t_interface_debug_print("rx8025t: max current is %0.2fmA.\n", info.max_current_ma);
        rx8025t_interface_debug_print("rx8025t: max temperature is %0.1fC.\n", info.temperature_max);
        rx8025t_interface_debug_print("rx8025t: min temperature is %0.1fC.\n", info.temperature_min);

        return 0;
    }
    else if (strcmp("p", type) == 0)
    {
        /* print pin connection */
        rx8025t_interface_debug_print("rx8025t: SCL connected to GPIO3(BCM).\n");
        rx8025t_interface_debug_print("rx8025t: SDA connected to GPIO2(BCM).\n");
        rx8025t_interface_debug_print("rx8025t: INT connected to GPIO17(BCM).\n");

        return 0;
    }
    else
    {
        return 5;
    }
}

/**
 * @brief     main function
 * @param[in] argc arg numbers
 * @param[in] **argv arg address
 * @return    status code
 *             - 0 success
 * @note      none
 */
int main(uint8_t argc, char **argv)
{
    uint8_t res;

    res = rx8025t(argc, argv);
    if (res == 0)
    {
        /* run success */
    }
    else if (res == 1)
    {
        rx8025t_interface_debug_print("rx8025t: run failed.\n");
    }
    else if (res == 5)
    {
        rx8025t_interface_debug_print("rx8025t: param is invalid.\n");
    }
    else
    {
        rx8025t_interface_debug_print("rx8025t: unknown status code.\n");
    }

    return 0;
}
