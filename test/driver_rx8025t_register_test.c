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
 * @file      driver_rx8025t_register_test.c
 * @brief     driver rx8025t register test source file
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
#include <stdlib.h>

static rx8025t_handle_t gs_handle;        /**< rx8025t handle */

/**
 * @brief  register test
 * @return status code
 *         - 0 success
 *         - 1 test failed
 * @note   none
 */
uint8_t rx8025t_register_test(void)
{
    uint8_t res;
    uint8_t data;
    uint8_t data_check;
    uint16_t counter;
    uint16_t counter_check;
    rx8025t_info_t info;
    rx8025t_time_t time_in;
    rx8025t_time_t time_out;
    rx8025t_bool_t enable;
    rx8025t_alarm_t alarm;
    rx8025t_timer_clock_t clk;
    rx8025t_update_select_t select;
    rx8025t_square_wave_frequency_t freq;
    rx8025t_temperature_compensation_interval_t interval;
    
    /* link functions */
    DRIVER_RX8025T_LINK_INIT(&gs_handle, rx8025t_handle_t);
    DRIVER_RX8025T_LINK_IIC_INIT(&gs_handle, rx8025t_interface_iic_init);
    DRIVER_RX8025T_LINK_IIC_DEINIT(&gs_handle, rx8025t_interface_iic_deinit);
    DRIVER_RX8025T_LINK_IIC_READ(&gs_handle, rx8025t_interface_iic_read);
    DRIVER_RX8025T_LINK_IIC_WRITE(&gs_handle, rx8025t_interface_iic_write);
    DRIVER_RX8025T_LINK_DELAY_MS(&gs_handle, rx8025t_interface_delay_ms);
    DRIVER_RX8025T_LINK_DEBUG_PRINT(&gs_handle, rx8025t_interface_debug_print);
    DRIVER_RX8025T_LINK_RECEIVE_CALLBACK(&gs_handle, rx8025t_interface_receive_callback);
    
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
    
    /* start register test */
    rx8025t_interface_debug_print("rx8025t: start register test.\n");
    
    /* init rx8025t */
    res = rx8025t_init(&gs_handle);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: init failed.\n");
        
        return 1;
    }
    
    /* rx8025t_start/rx8025t_stop test */
    rx8025t_interface_debug_print("rx8025t: rx8025t_start/rx8025t_stop test.\n");
    
    /* stop */
    res = rx8025t_stop(&gs_handle);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: stop failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: stop.\n");
    
    /* output */
    rx8025t_interface_debug_print("rx8025t: check stop %s.\n", (res == 0) ? "ok" : "error");
    
    /* start */
    res = rx8025t_start(&gs_handle);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: start failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: start.\n");
    
    /* output */
    rx8025t_interface_debug_print("rx8025t: check start %s.\n", (res == 0) ? "ok" : "error");
    
    /* rx8025t_set_time/rx8025t_get_time test */
    rx8025t_interface_debug_print("rx8025t: rx8025t_set_time/rx8025t_get_time test.\n");
    
    time_in.year = rand() % 100 + 2000;
    time_in.month = rand() % 12 + 1;
    time_in.date = rand() % 20 + 1;
    time_in.week = rand() % 7 + 1;
    time_in.hour = rand() % 24;
    time_in.minute = rand() % 60;
    time_in.second = rand() % 60;
    res  = rx8025t_set_time(&gs_handle, &time_in);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: set time failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: set time %04d-%02d-%02d %02d:%02d:%02d %d.\n",
                                  time_in.year, time_in.month, time_in.date,
                                  time_in.hour, time_in.minute, time_in.second, time_in.week
                                 );
    res = rx8025t_get_time(&gs_handle, &time_out);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: get time failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: check time %s.\n", (memcmp((uint8_t *)&time_in, (uint8_t *)&time_out, sizeof(rx8025t_time_t)) == 0) ? "ok" : "error");
    
    /* rx8025t_set_temperature_compensation_interval/rx8025t_get_temperature_compensation_interval test */
    rx8025t_interface_debug_print("rx8025t: rx8025t_set_temperature_compensation_interval/rx8025t_get_temperature_compensation_interval test.\n");
    
    /* set temperature compensation interval 0.5s */
    res = rx8025t_set_temperature_compensation_interval(&gs_handle, RX8025T_TEMPERATURE_COMPENSATION_INTERVAL_0P5_SECOND);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: set temperature compensation interval failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: set temperature compensation interval 0.5s.\n");
    res = rx8025t_get_temperature_compensation_interval(&gs_handle, &interval);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: get temperature compensation interval failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: check temperature compensation interval %s.\n",
                                 (interval == RX8025T_TEMPERATURE_COMPENSATION_INTERVAL_0P5_SECOND) ? "ok" : "error");
    
    /* set temperature compensation interval 2.0s */
    res = rx8025t_set_temperature_compensation_interval(&gs_handle, RX8025T_TEMPERATURE_COMPENSATION_INTERVAL_2P0_SECOND);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: set temperature compensation interval failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: set temperature compensation interval 2.0s.\n");
    res = rx8025t_get_temperature_compensation_interval(&gs_handle, &interval);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: get temperature compensation interval failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: check temperature compensation interval %s.\n",
                                 (interval == RX8025T_TEMPERATURE_COMPENSATION_INTERVAL_2P0_SECOND) ? "ok" : "error");
    
    /* set temperature compensation interval 10s */
    res = rx8025t_set_temperature_compensation_interval(&gs_handle, RX8025T_TEMPERATURE_COMPENSATION_INTERVAL_10_SECOND);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: set temperature compensation interval failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: set temperature compensation interval 10s.\n");
    res = rx8025t_get_temperature_compensation_interval(&gs_handle, &interval);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: get temperature compensation interval failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: check temperature compensation interval %s.\n",
                                 (interval == RX8025T_TEMPERATURE_COMPENSATION_INTERVAL_10_SECOND) ? "ok" : "error");
    
    /* set temperature compensation interval 30s */
    res = rx8025t_set_temperature_compensation_interval(&gs_handle, RX8025T_TEMPERATURE_COMPENSATION_INTERVAL_30_SECOND);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: set temperature compensation interval failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: set temperature compensation interval 30s.\n");
    res = rx8025t_get_temperature_compensation_interval(&gs_handle, &interval);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: get temperature compensation interval failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: check temperature compensation interval %s.\n",
                                 (interval == RX8025T_TEMPERATURE_COMPENSATION_INTERVAL_30_SECOND) ? "ok" : "error");
    
    /* rx8025t_set_update_interrupt/rx8025t_get_update_interrupt test */
    rx8025t_interface_debug_print("rx8025t: rx8025t_set_update_interrupt/rx8025t_get_update_interrupt test.\n");
    
    /* enable update interrupt */
    res = rx8025t_set_update_interrupt(&gs_handle, RX8025T_BOOL_TRUE);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: set update interrupt failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: enable update interrupt.\n");
    res = rx8025t_get_update_interrupt(&gs_handle, &enable);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: get update interrupt failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: check update interrupt %s.\n", (enable == RX8025T_BOOL_TRUE) ? "ok" : "error");
    
    /* disable update interrupt */
    res = rx8025t_set_update_interrupt(&gs_handle, RX8025T_BOOL_FALSE);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: set update interrupt failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: disable update interrupt.\n");
    res = rx8025t_get_update_interrupt(&gs_handle, &enable);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: get update interrupt failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: check update interrupt %s.\n", (enable == RX8025T_BOOL_FALSE) ? "ok" : "error");
    
    /* rx8025t_set_timer_interrupt/rx8025t_get_timer_interrupt test */
    rx8025t_interface_debug_print("rx8025t: rx8025t_set_timer_interrupt/rx8025t_get_timer_interrupt test.\n");
    
    /* enable timer interrupt */
    res = rx8025t_set_timer_interrupt(&gs_handle, RX8025T_BOOL_TRUE);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: set timer interrupt failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: enable timer interrupt.\n");
    res = rx8025t_get_timer_interrupt(&gs_handle, &enable);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: get timer interrupt failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: check timer interrupt %s.\n", (enable == RX8025T_BOOL_TRUE) ? "ok" : "error");
    
    /* disable timer interrupt */
    res = rx8025t_set_timer_interrupt(&gs_handle, RX8025T_BOOL_FALSE);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: set timer interrupt failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: disable timer interrupt.\n");
    res = rx8025t_get_timer_interrupt(&gs_handle, &enable);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: get timer interrupt failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: check timer interrupt %s.\n", (enable == RX8025T_BOOL_FALSE) ? "ok" : "error");
    
    /* rx8025t_set_alarm_interrupt/rx8025t_get_alarm_interrupt test */
    rx8025t_interface_debug_print("rx8025t: rx8025t_set_alarm_interrupt/rx8025t_get_alarm_interrupt test.\n");
    
    /* enable alarm interrupt */
    res = rx8025t_set_alarm_interrupt(&gs_handle, RX8025T_BOOL_TRUE);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: set alarm interrupt failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: enable alarm interrupt.\n");
    res = rx8025t_get_alarm_interrupt(&gs_handle, &enable);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: get alarm interrupt failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: check alarm interrupt %s.\n", (enable == RX8025T_BOOL_TRUE) ? "ok" : "error");
    
    /* disable alarm interrupt */
    res = rx8025t_set_alarm_interrupt(&gs_handle, RX8025T_BOOL_FALSE);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: set alarm interrupt failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: disable alarm interrupt.\n");
    res = rx8025t_get_alarm_interrupt(&gs_handle, &enable);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: get alarm interrupt failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: check alarm interrupt %s.\n", (enable == RX8025T_BOOL_FALSE) ? "ok" : "error");
    
    /* rx8025t_set_test/rx8025t_get_test test */
    rx8025t_interface_debug_print("rx8025t: rx8025t_set_test/rx8025t_get_test test.\n");
    
    /* enable test */
    res = rx8025t_set_test(&gs_handle, RX8025T_BOOL_TRUE);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: set test failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: enable test.\n");
    res = rx8025t_get_test(&gs_handle, &enable);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: get test failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: check test %s.\n", (enable == RX8025T_BOOL_TRUE) ? "ok" : "error");
    
    /* disable test */
    res = rx8025t_set_test(&gs_handle, RX8025T_BOOL_FALSE);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: set test failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: disable test.\n");
    res = rx8025t_get_test(&gs_handle, &enable);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: get test failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: check test %s.\n", (enable == RX8025T_BOOL_FALSE) ? "ok" : "error");
    
    /* rx8025t_set_test/rx8025t_get_test test */
    rx8025t_interface_debug_print("rx8025t: rx8025t_set_test/rx8025t_get_test test.\n");
    
    /* set alarm week */
    res = rx8025t_set_alarm_day_week(&gs_handle, RX8025T_ALARM_WEEK);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: set alarm day week failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: set alarm week.\n");
    res = rx8025t_get_alarm_day_week(&gs_handle, &alarm);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: get alarm day week failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: check alarm day week %s.\n", (alarm == RX8025T_ALARM_WEEK) ? "ok" : "error");
    
    /* set alarm day */
    res = rx8025t_set_alarm_day_week(&gs_handle, RX8025T_ALARM_DAY);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: set alarm day week failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: set alarm day.\n");
    res = rx8025t_get_alarm_day_week(&gs_handle, &alarm);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: get alarm day week failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: check alarm day week %s.\n", (alarm == RX8025T_ALARM_DAY) ? "ok" : "error");
    
    /* rx8025t_set_update_select/rx8025t_get_update_select test */
    rx8025t_interface_debug_print("rx8025t: rx8025t_set_update_select/rx8025t_get_update_select test.\n");
    
    /* set update select second */
    res = rx8025t_set_update_select(&gs_handle, RX8025T_UPDATE_SELECT_SECOND);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: set update select failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: set update select second.\n");
    res = rx8025t_get_update_select(&gs_handle, &select);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: get update select failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: check update select %s.\n", (select == RX8025T_UPDATE_SELECT_SECOND) ? "ok" : "error");
    
    /* set update select minute */
    res = rx8025t_set_update_select(&gs_handle, RX8025T_UPDATE_SELECT_MINUTE);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: set update select failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: set update select minute.\n");
    res = rx8025t_get_update_select(&gs_handle, &select);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: get update select failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: check update select %s.\n", (select == RX8025T_UPDATE_SELECT_MINUTE) ? "ok" : "error");
    
    /* rx8025t_set_timer/rx8025t_get_timer test */
    rx8025t_interface_debug_print("rx8025t: rx8025t_set_timer/rx8025t_get_timer test.\n");
    
    /* enable timer */
    res = rx8025t_set_timer(&gs_handle, RX8025T_BOOL_TRUE);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: set timer failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: enable timer.\n");
    res = rx8025t_get_timer(&gs_handle, &enable);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: get timer failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: check timer %s.\n", (enable == RX8025T_BOOL_TRUE) ? "ok" : "error");
    
    /* disable timer */
    res = rx8025t_set_timer(&gs_handle, RX8025T_BOOL_FALSE);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: set timer failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: disable timer.\n");
    res = rx8025t_get_timer(&gs_handle, &enable);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: get timer failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: check timer %s.\n", (enable == RX8025T_BOOL_FALSE) ? "ok" : "error");
    
    /* rx8025t_set_square_wave_frequency/rx8025t_get_square_wave_frequency test */
    rx8025t_interface_debug_print("rx8025t: rx8025t_set_square_wave_frequency/rx8025t_get_square_wave_frequency test.\n");
    
    /* set square wave frequency 1hz */
    res = rx8025t_set_square_wave_frequency(&gs_handle, RX8025T_SQUARE_WAVE_FREQUENCY_1_HZ);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: set square wave frequency failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: set square wave frequency 1hz.\n");
    res = rx8025t_get_square_wave_frequency(&gs_handle, &freq);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: get square wave frequency failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: check square wave frequency %s.\n", (freq == RX8025T_SQUARE_WAVE_FREQUENCY_1_HZ) ? "ok" : "error");
    
    /* set square wave frequency 1024hz */
    res = rx8025t_set_square_wave_frequency(&gs_handle, RX8025T_SQUARE_WAVE_FREQUENCY_1024_HZ);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: set square wave frequency failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: set square wave frequency 1024hz.\n");
    res = rx8025t_get_square_wave_frequency(&gs_handle, &freq);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: get square wave frequency failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: check square wave frequency %s.\n", (freq == RX8025T_SQUARE_WAVE_FREQUENCY_1024_HZ) ? "ok" : "error");
    
    /* set square wave frequency 32768Hz */
    res = rx8025t_set_square_wave_frequency(&gs_handle, RX8025T_SQUARE_WAVE_FREQUENCY_32768_HZ);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: set square wave frequency failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: set square wave frequency 32768Hz.\n");
    res = rx8025t_get_square_wave_frequency(&gs_handle, &freq);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: get square wave frequency failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: check square wave frequency %s.\n", (freq == RX8025T_SQUARE_WAVE_FREQUENCY_32768_HZ) ? "ok" : "error");
    
    /* rx8025t_set_timer_clock/rx8025t_get_timer_clock test */
    rx8025t_interface_debug_print("rx8025t: rx8025t_set_timer_clock/rx8025t_get_timer_clock test.\n");
    
    /* set timer clock 4096hz */
    res = rx8025t_set_timer_clock(&gs_handle, RX8025T_TIMER_CLOCK_4096_HZ);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: set timer clock failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: set timer clock 4096hz.\n");
    res = rx8025t_get_timer_clock(&gs_handle, &clk);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: get timer clock failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: check timer clock %s.\n", (clk == RX8025T_TIMER_CLOCK_4096_HZ) ? "ok" : "error");
    
    /* set timer clock 64hz */
    res = rx8025t_set_timer_clock(&gs_handle, RX8025T_TIMER_CLOCK_64_HZ);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: set timer clock failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: set timer clock 64hz.\n");
    res = rx8025t_get_timer_clock(&gs_handle, &clk);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: get timer clock failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: check timer clock %s.\n", (clk == RX8025T_TIMER_CLOCK_64_HZ) ? "ok" : "error");
    
    /* set timer clock 1hz */
    res = rx8025t_set_timer_clock(&gs_handle, RX8025T_TIMER_CLOCK_1_HZ);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: set timer clock failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: set timer clock 1hz.\n");
    res = rx8025t_get_timer_clock(&gs_handle, &clk);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: get timer clock failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: check timer clock %s.\n", (clk == RX8025T_TIMER_CLOCK_1_HZ) ? "ok" : "error");
    
    /* set timer clock 1 minute */
    res = rx8025t_set_timer_clock(&gs_handle, RX8025T_TIMER_CLOCK_MINUTE);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: set timer clock failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: set timer clock 1 minute.\n");
    res = rx8025t_get_timer_clock(&gs_handle, &clk);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: get timer clock failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: check timer clock %s.\n", (clk == RX8025T_TIMER_CLOCK_MINUTE) ? "ok" : "error");
    
    /* rx8025t_set_timer_counter/rx8025t_get_timer_counter test */
    rx8025t_interface_debug_print("rx8025t: rx8025t_set_timer_counter/rx8025t_get_timer_counter test.\n");
    
    counter = rand() % 0x0FFFU;
    res = rx8025t_set_timer_counter(&gs_handle, counter);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: set timer counter failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: set timer counter %d.\n", counter);
    res = rx8025t_get_timer_counter(&gs_handle, &counter_check);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: get timer counter failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: check timer counter %s.\n", (counter == counter_check) ? "ok" : "error");
    
    /* rx8025t_set_ram/rx8025t_get_ram test */
    rx8025t_interface_debug_print("rx8025t: rx8025t_set_ram/rx8025t_get_ram test.\n");
    
    data = rand() % 0xFFU;
    res = rx8025t_set_ram(&gs_handle, data);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: set ram failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: set ram %d.\n", data);
    res = rx8025t_get_ram(&gs_handle, &data_check);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: get ram failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: check ram %s.\n", (data == data_check) ? "ok" : "error");
    
    /* rx8025t_set_minute_alarm/rx8025t_get_minute_alarm test */
    rx8025t_interface_debug_print("rx8025t: rx8025t_set_minute_alarm/rx8025t_get_minute_alarm test.\n");
    
    data = rand() % 60;
    res = rx8025t_set_minute_alarm(&gs_handle, data);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: set minute alarm failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: set minute alarm %d.\n", data);
    res = rx8025t_get_minute_alarm(&gs_handle, &data_check);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: get minute alarm failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: check minute alarm %s.\n", (data == data_check) ? "ok" : "error");
    
    /* rx8025t_set_hour_alarm/rx8025t_get_hour_alarm test */
    rx8025t_interface_debug_print("rx8025t: rx8025t_set_hour_alarm/rx8025t_get_hour_alarm test.\n");
    
    data = rand() % 24;
    res = rx8025t_set_hour_alarm(&gs_handle, data);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: set hour alarm failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: set hour alarm %d.\n", data);
    res = rx8025t_get_hour_alarm(&gs_handle, &data_check);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: get hour alarm failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: check hour alarm %s.\n", (data == data_check) ? "ok" : "error");
    
    /* rx8025t_set_date_alarm/rx8025t_get_date_alarm test */
    rx8025t_interface_debug_print("rx8025t: rx8025t_set_date_alarm/rx8025t_get_date_alarm test.\n");
    
    data = rand() % 20 + 1;
    res = rx8025t_set_date_alarm(&gs_handle, data);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: set date alarm failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: set date alarm %d.\n", data);
    res = rx8025t_get_date_alarm(&gs_handle, &data_check);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: get date alarm failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: check date alarm %s.\n", (data == data_check) ? "ok" : "error");
    
    /* rx8025t_set_week_alarm/rx8025t_get_week_alarm test */
    rx8025t_interface_debug_print("rx8025t: rx8025t_set_week_alarm/rx8025t_get_week_alarm test.\n");
    
    data = rand() % 7 + 1;
    res = rx8025t_set_week_alarm(&gs_handle, data);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: set week alarm failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: set week alarm %d.\n", data);
    res = rx8025t_get_week_alarm(&gs_handle, &data_check);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: get week alarm failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: check week alarm %s.\n", (data == data_check) ? "ok" : "error");
    
    /* rx8025t_clear_flag test */
    rx8025t_interface_debug_print("rx8025t: rx8025t_clear_flag test.\n");
    
    /* clear flag */
    res = rx8025t_clear_flag(&gs_handle);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: clear flag failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: check clear flag %s.\n", (res == 0) ? "ok" : "error");
    
    /* rx8025t_get_flag test */
    rx8025t_interface_debug_print("rx8025t: rx8025t_get_flag test.\n");
    
    /* get flag */
    res = rx8025t_get_flag(&gs_handle, &data_check);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: get flag failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: flag is 0x%02X.\n", data_check);
    
    /* rx8025t_set_minute_alarm_mask/rx8025t_get_minute_alarm_mask test */
    rx8025t_interface_debug_print("rx8025t: rx8025t_set_minute_alarm_mask/rx8025t_get_minute_alarm_mask test.\n");
    
    /* enable minute alarm mask */
    res = rx8025t_set_minute_alarm_mask(&gs_handle, RX8025T_BOOL_TRUE);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: set minute alarm mask failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: enable minute alarm mask.\n");
    res = rx8025t_get_minute_alarm_mask(&gs_handle, &enable);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: get minute alarm mask failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: check minute alarm mask %s.\n", (enable == RX8025T_BOOL_TRUE) ? "ok" : "error");
    
    /* disable minute alarm mask */
    res = rx8025t_set_minute_alarm_mask(&gs_handle, RX8025T_BOOL_FALSE);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: set minute alarm mask failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: disable minute alarm mask.\n");
    res = rx8025t_get_minute_alarm_mask(&gs_handle, &enable);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: get minute alarm mask failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: check minute alarm mask %s.\n", (enable == RX8025T_BOOL_FALSE) ? "ok" : "error");
    
    /* rx8025t_set_hour_alarm_mask/rx8025t_get_hour_alarm_mask test */
    rx8025t_interface_debug_print("rx8025t: rx8025t_set_hour_alarm_mask/rx8025t_get_hour_alarm_mask test.\n");
    
    /* enable hour alarm mask */
    res = rx8025t_set_hour_alarm_mask(&gs_handle, RX8025T_BOOL_TRUE);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: set hour alarm mask failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: enable hour alarm mask.\n");
    res = rx8025t_get_hour_alarm_mask(&gs_handle, &enable);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: get hour alarm mask failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: check hour alarm mask %s.\n", (enable == RX8025T_BOOL_TRUE) ? "ok" : "error");
    
    /* disable hour alarm mask */
    res = rx8025t_set_hour_alarm_mask(&gs_handle, RX8025T_BOOL_FALSE);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: set hour alarm mask failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: disable hour alarm mask.\n");
    res = rx8025t_get_hour_alarm_mask(&gs_handle, &enable);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: get hour alarm mask failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: check hour alarm mask %s.\n", (enable == RX8025T_BOOL_FALSE) ? "ok" : "error");
    
    /* rx8025t_set_week_day_alarm_mask/rx8025t_get_week_day_alarm_mask test */
    rx8025t_interface_debug_print("rx8025t: rx8025t_set_week_day_alarm_mask/rx8025t_get_week_day_alarm_mask test.\n");
    
    /* enable week day alarm mask */
    res = rx8025t_set_week_day_alarm_mask(&gs_handle, RX8025T_BOOL_TRUE);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: set week day alarm mask failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: enable week day alarm mask.\n");
    res = rx8025t_get_week_day_alarm_mask(&gs_handle, &enable);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: get week day alarm mask failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: check week day alarm mask %s.\n", (enable == RX8025T_BOOL_TRUE) ? "ok" : "error");
    
    /* disable week day alarm mask */
    res = rx8025t_set_week_day_alarm_mask(&gs_handle, RX8025T_BOOL_FALSE);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: set week day alarm mask failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: disable week day alarm mask.\n");
    res = rx8025t_get_week_day_alarm_mask(&gs_handle, &enable);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: get week day alarm mask failed.\n");
        (void)rx8025t_deinit(&gs_handle);
        
        return 1;
    }
    rx8025t_interface_debug_print("rx8025t: check week day alarm mask %s.\n", (enable == RX8025T_BOOL_FALSE) ? "ok" : "error");
    
    /* finish register test */
    rx8025t_interface_debug_print("rx8025t: finish register test.\n");
    (void)rx8025t_deinit(&gs_handle);
    
    return 0;
}
