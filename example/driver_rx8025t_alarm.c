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
 * @file      driver_rx8025t_alarm.c
 * @brief     driver rx8025t alarm source file
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

#include "driver_rx8025t_alarm.h"

static rx8025t_handle_t gs_handle;        /**< rx8025t handle */
static int8_t gs_time_zone = 0;           /**< local zone */

/**
 * @brief  alarm irq
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t rx8025t_alarm_irq_handler(void)
{
    if (rx8025t_irq_handler(&gs_handle) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief     alarm example init
 * @param[in] *callback pointer to a callback function address
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t rx8025t_alarm_init(void (*callback)(uint8_t type))
{
    uint8_t res;

    /* link functions */
    DRIVER_RX8025T_LINK_INIT(&gs_handle, rx8025t_handle_t);
    DRIVER_RX8025T_LINK_IIC_INIT(&gs_handle, rx8025t_interface_iic_init);
    DRIVER_RX8025T_LINK_IIC_DEINIT(&gs_handle, rx8025t_interface_iic_deinit);
    DRIVER_RX8025T_LINK_IIC_READ(&gs_handle, rx8025t_interface_iic_read);
    DRIVER_RX8025T_LINK_IIC_WRITE(&gs_handle, rx8025t_interface_iic_write);
    DRIVER_RX8025T_LINK_DELAY_MS(&gs_handle, rx8025t_interface_delay_ms);
    DRIVER_RX8025T_LINK_DEBUG_PRINT(&gs_handle, rx8025t_interface_debug_print);
    DRIVER_RX8025T_LINK_RECEIVE_CALLBACK(&gs_handle, callback);

    /* init rx8025t */
    res = rx8025t_init(&gs_handle);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: init failed.\n");

        return 1;
    }

    /* start the clock */
    res = rx8025t_start(&gs_handle);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: start failed.\n");
        (void)rx8025t_deinit(&gs_handle);

        return 1;
    }
    
    /* set default temperature compensation interval */
    res = rx8025t_set_temperature_compensation_interval(&gs_handle, RX8025T_ALARM_DEFAULT_TEMPERATURE_COMPENSATION_INTERVAL);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: set temperature compensation interval failed.\n");
        (void)rx8025t_deinit(&gs_handle);

        return 1;
    }
    
    /* set default update interrupt */
    res = rx8025t_set_update_interrupt(&gs_handle, RX8025T_ALARM_DEFAULT_INTERRUPT_UPDATE);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: set update interrupt failed.\n");
        (void)rx8025t_deinit(&gs_handle);

        return 1;
    }
    
    /* set default update select */
    res = rx8025t_set_update_select(&gs_handle, RX8025T_ALARM_DEFAULT_UPDATE_SELECT);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: set update select failed.\n");
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
    
    /* enable alarm interrupt */
    res = rx8025t_set_alarm_interrupt(&gs_handle, RX8025T_BOOL_TRUE);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: set alarm interrupt failed.\n");
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
    
    /* disable test */
    res = rx8025t_set_test(&gs_handle, RX8025T_BOOL_FALSE);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: set test failed.\n");
        (void)rx8025t_deinit(&gs_handle);

        return 1;
    }
    
    /* disable timer */
    res = rx8025t_set_timer(&gs_handle, RX8025T_BOOL_FALSE);
    if (res != 0)
    {
        rx8025t_interface_debug_print("rx8025t: set timer failed.\n");
        (void)rx8025t_deinit(&gs_handle);

        return 1;
    }
    
    return 0;
}

/**
 * @brief  alarm example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t rx8025t_alarm_deinit(void)
{
    if (rx8025t_deinit(&gs_handle) != 0)
    {
        return 1;
    }

    return 0;
}

/**
 * @brief     alarm example config
 * @param[in] minute alarm minute
 * @param[in] hour alarm hour
 * @param[in] date_week date week select
 * @param[in] date alarm date
 * @param[in] week alarm week
 * @param[in] minute_mask bool value
 * @param[in] hour_mask bool value
 * @param[in] day_week_mask bool value
 * @return    status code
 *            - 0 success
 *            - 1 config failed
 * @note      none
 */
uint8_t rx8025t_alarm_config(uint8_t minute, uint8_t hour, 
                             rx8025t_alarm_t date_week, uint8_t date, uint8_t week,
                             rx8025t_bool_t minute_mask, rx8025t_bool_t hour_mask, rx8025t_bool_t day_week_mask)
{
    uint8_t res;
    
    /* set minute alarm */
    res = rx8025t_set_minute_alarm(&gs_handle, minute);
    if (res != 0)
    {
        return 1;
    }
    
    /* set hour alarm */
    res = rx8025t_set_hour_alarm(&gs_handle, hour);
    if (res != 0)
    {
        return 1;
    }
    
    /* set alarm day week */
    res = rx8025t_set_alarm_day_week(&gs_handle, date_week);
    if (res != 0)
    {
        return 1;
    }
    
    /* set date alarm */
    res = rx8025t_set_date_alarm(&gs_handle, date);
    if (res != 0)
    {
        return 1;
    }
    
    /* set week alarm */
    res = rx8025t_set_week_alarm(&gs_handle, week);
    if (res != 0)
    {
        return 1;
    }
    
    /* set minute alarm mask */
    res = rx8025t_set_minute_alarm_mask(&gs_handle, minute_mask);
    if (res != 0)
    {
        return 1;
    }
    
    /* set hour alarm mask */
    res = rx8025t_set_hour_alarm_mask(&gs_handle, hour_mask);
    if (res != 0)
    {
        return 1;
    }
    
    /* set week day alarm mask */
    res = rx8025t_set_week_day_alarm_mask(&gs_handle, day_week_mask);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief     alarm example set the time
 * @param[in] *t pointer to a time structure
 * @return    status code
 *            - 0 success
 *            - 1 set time failed
 * @note      none
 */
uint8_t rx8025t_alarm_set_time(rx8025t_time_t *t)
{
    /* set time */
    if (rx8025t_set_time(&gs_handle, t) != 0)
    {
        return 1;
    }

    return 0;
}

/**
 * @brief     alarm example set the time by a unix timestamp
 * @param[in] timestamp unix timestamp
 * @return    status code
 *            - 0 success
 *            - 1 set timestamp failed
 * @note      none
 */
uint8_t rx8025t_alarm_set_timestamp(time_t timestamp)
{
    rx8025t_time_t t;
    struct tm *timeptr;

    /* convert times */
    timestamp += (time_t)(gs_time_zone * 3600);
    timeptr = localtime(&timestamp);
    t.date = (uint8_t)timeptr->tm_mday;
    t.hour = (uint8_t)timeptr->tm_hour;
    t.minute = (uint8_t)timeptr->tm_min;
    t.month = (uint8_t)timeptr->tm_mon + 1;
    t.second = (uint8_t)timeptr->tm_sec;
    if (timeptr->tm_wday == 0)
    {
        t.week  = 7;
    }
    else
    {
        t.week = (uint8_t)timeptr->tm_wday;
    }
    t.year = (uint16_t)(timeptr->tm_year + 1900);

    /* set time */
    if (rx8025t_set_time(&gs_handle, &t) != 0)
    {
        return 1;
    }

    return 0;
}

/**
 * @brief     alarm example set the local time zone
 * @param[in] zone local time zone
 * @return    status code
 *            - 0 success
 * @note      none
 */
uint8_t rx8025t_alarm_set_timestamp_time_zone(int8_t zone)
{
    gs_time_zone = zone;

    return 0;
}

/**
 * @brief      alarm example get the time
 * @param[out] *t pointer to a time structure
 * @return     status code
 *             - 0 success
 *             - 1 get time failed
 * @note       none
 */
uint8_t rx8025t_alarm_get_time(rx8025t_time_t *t)
{
    /* get time */
    if (rx8025t_get_time(&gs_handle, t) != 0)
    {
        return 1;
    }

    return 0;
}

/**
 * @brief      alarm example get the time in a unix timestamp
 * @param[out] *timestamp pointer to a unix timestamp buffer
 * @return     status code
 *             - 0 success
 *             - 1 get timestamp failed
 * @note       none
 */
uint8_t rx8025t_alarm_get_timestamp(time_t *timestamp)
{
    rx8025t_time_t t;
    struct tm timeptr;

    /* get time */
    if (rx8025t_get_time(&gs_handle, &t) != 0)
    {
        return 1;
    }
    timeptr.tm_year = t.year - 1900;
    timeptr.tm_mon = t.month - 1;
    timeptr.tm_wday = t.week;
    timeptr.tm_mday = t.date;
    timeptr.tm_hour = t.hour;
    timeptr.tm_min = t.minute;
    timeptr.tm_sec = t.second;

    /* make time */
    *timestamp = mktime(&timeptr) - gs_time_zone * 3600;

    return 0;
}

/**
 * @brief      alarm example get the local time zone
 * @param[out] *zone pointer to a local time zone buffer
 * @return     status code
 *             - 0 success
 * @note       none
 */
uint8_t rx8025t_alarm_get_timestamp_time_zone(int8_t *zone)
{
    *zone = gs_time_zone;

    return 0;
}

/**
 * @brief      alarm example get the ascii time
 * @param[out] *buf pointer to an ascii buffer
 * @param[in]  len data length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t rx8025t_alarm_get_ascii_time(char *buf, uint8_t len)
{
    rx8025t_time_t t;

    /* get time */
    if (rx8025t_get_time(&gs_handle, &t) != 0)
    {
        return 1;
    }
    
    (void)snprintf(buf, len, "%04d-%02d-%02d %02d:%02d:%02d %d.\n", t.year, t.month, t.date, t.hour, t.minute, t.second, t.week);
    
    return 0;
}

/**
 * @brief      alarm example read ram
 * @param[out] *data pointer to a ram data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read ram failed
 * @note       none
 */
uint8_t rx8025t_alarm_read_ram(uint8_t *data)
{
    if (rx8025t_get_ram(&gs_handle, data) != 0)
    {
        return 1;
    }

    return 0;
}

/**
 * @brief     alarm example write ram
 * @param[in] data set data
 * @return    status code
 *            - 0 success
 *            - 1 write ram failed
 * @note      none
 */
uint8_t rx8025t_alarm_write_ram(uint8_t data)
{
    if (rx8025t_set_ram(&gs_handle, data) != 0)
    {
        return 1;
    }

    return 0;
}
