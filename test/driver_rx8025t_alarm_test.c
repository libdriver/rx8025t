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
 * @file      driver_rx8025t_alarm_test.c
 * @brief     driver rx8025t alarm test source file
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

#include "driver_rx8025t_alarm_test.h"
#include <stdlib.h>

static rx8025t_handle_t gs_handle;        /**< rx8025t handle */
static uint8_t gs_flag;                   /**< flag */

/**
 * @brief     interface receive callback
 * @param[in] type irq type
 * @note      none
 */
static void a_callback(uint8_t type)
{
    switch (type)
    {
        case RX8025T_STATUS_UPDATE :
        {
            rx8025t_interface_debug_print("rx8025t: irq update.\n");
            
            break;
        }
        case RX8025T_STATUS_TIMER :
        {
            rx8025t_interface_debug_print("rx8025t: irq timer.\n");
            
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
 * @brief  alarm test irq
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t rx8025t_alarm_test_irq_handler(void)
{
    if (rx8025t_irq_handler(&gs_handle) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief  alarm test
 * @return status code
 *         - 0 success
 *         - 1 test failed
 * @note   none
 */
uint8_t rx8025t_alarm_test(void)
{
    uint8_t res;
    uint16_t timeout;
    rx8025t_info_t info;
    rx8025t_time_t t;
    
    /* link functions */
    DRIVER_RX8025T_LINK_INIT(&gs_handle, rx8025t_handle_t);
    DRIVER_RX8025T_LINK_IIC_INIT(&gs_handle, rx8025t_interface_iic_init);
    DRIVER_RX8025T_LINK_IIC_DEINIT(&gs_handle, rx8025t_interface_iic_deinit);
    DRIVER_RX8025T_LINK_IIC_READ(&gs_handle, rx8025t_interface_iic_read);
    DRIVER_RX8025T_LINK_IIC_WRITE(&gs_handle, rx8025t_interface_iic_write);
    DRIVER_RX8025T_LINK_DELAY_MS(&gs_handle, rx8025t_interface_delay_ms);
    DRIVER_RX8025T_LINK_DEBUG_PRINT(&gs_handle, rx8025t_interface_debug_print);
    DRIVER_RX8025T_LINK_RECEIVE_CALLBACK(&gs_handle, a_callback);
    
    /* get rx8025t info */
    res = rx8025t_info(&info);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: get info failed.\n");
       
        return 1;
    }
    else
    {
        /* print rx8025t info */
        rx8025t_interface_debug_print("rx8025t: chip is %s.\n", info.chip_name);
        rx8025t_interface_debug_print("rx8025t: manufacturer is %s.\n", info.manufacturer_name);
        rx8025t_interface_debug_print("rx8025t: interface is %s.\n", info.interface);
        rx8025t_interface_debug_print("rx8025t: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        rx8025t_interface_debug_print("rx8025t: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        rx8025t_interface_debug_print("rx8025t: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        rx8025t_interface_debug_print("rx8025t: max current is %0.2fmA.\n", info.max_current_ma);
        rx8025t_interface_debug_print("rx8025t: max temperature is %0.1fC.\n", info.temperature_max);
        rx8025t_interface_debug_print("rx8025t: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
    /* start alarm test */
    rx8025t_interface_debug_print("rx8025t: start alarm test.\n");
    
    /* init rx8025t */
    res = rx8025t_init(&gs_handle);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: init failed.\n");
        
        return 1;
    }
    
    /* start */
    res = rx8025t_start(&gs_handle);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: start failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set alarm day */
    res = rx8025t_set_alarm_day_week(&gs_handle, RX8025T_ALARM_DAY);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: set alarm day week failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    
    t.year = rand() % 100 + 2000;
    t.month = rand() % 12 + 1;
    t.date = rand() % 20 + 1;
    t.week = rand() % 7 + 1;
    t.hour = rand() % 24;
    t.minute = 0;
    t.second = 0;
    res  = rx8025t_set_time(&gs_handle, &t);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: set time failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: set time %04d-%02d-%02d %02d:%02d:%02d %d.\n",
                                  t.year, t.month, t.date,
                                  t.hour, t.minute, t.second, t.week
                                 );
    
    /* 1 minute alarm */
    res = rx8025t_set_minute_alarm(&gs_handle, 1);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: set minute alarm failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable minute alarm mask */
    res = rx8025t_set_minute_alarm_mask(&gs_handle, RX8025T_BOOL_FALSE);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: set minute alarm mask failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable hour alarm mask */
    res = rx8025t_set_hour_alarm_mask(&gs_handle, RX8025T_BOOL_TRUE);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: set hour alarm mask failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable week day alarm mask */
    res = rx8025t_set_week_day_alarm_mask(&gs_handle, RX8025T_BOOL_TRUE);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: set week day alarm mask failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set temperature compensation interval */
    res = rx8025t_set_temperature_compensation_interval(&gs_handle, RX8025T_TEMPERATURE_COMPENSATION_INTERVAL_0P5_SECOND);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: set temperature compensation interval failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    
    /* second update */
    res = rx8025t_set_update_select(&gs_handle, RX8025T_UPDATE_SELECT_SECOND);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: set update select failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable test */
    res = rx8025t_set_test(&gs_handle, RX8025T_BOOL_FALSE);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: set test failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable timer interrupt */
    res = rx8025t_set_timer_interrupt(&gs_handle, RX8025T_BOOL_FALSE);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: set timer interrupt failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable update interrupt */
    res = rx8025t_set_update_interrupt(&gs_handle, RX8025T_BOOL_TRUE);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: set update interrupt failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    
    /* clear flag */
    res = rx8025t_clear_flag(&gs_handle);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: clear flag failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable alarm interrupt */
    res = rx8025t_set_alarm_interrupt(&gs_handle, RX8025T_BOOL_TRUE);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: set alarm interrupt failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    rx8025t_interface_debug_print("rx8025t: 1 min alarm start.\n");
    
    gs_flag = 0;
    timeout = 1000 * 65;
    while (timeout != 0)
    {
        /* check flag */
        if (gs_flag != 0)
        {
            break;
        }
        
        /* timeout-- */
        timeout--;
        
        /* delay 1ms */
        rx8025t_interface_delay_ms(1);
    }
    
    /* check timeout */
    if (timeout != 0)
    {
        rx8025t_interface_debug_print("rx8025t: end.\n");
    }
    else
    {
        rx8025t_interface_debug_print("rx8025t: timeout.\n");
    }
    
    /* finish alarm test */
    rx8025t_interface_debug_print("rx8025t: finish alarm test.\n");
    (void)rx8025t_set_alarm_interrupt(&gs_handle, RX8025T_BOOL_FALSE);
    (void)rx8025t_deinit(&gs_handle);
    
    return 0;
}
