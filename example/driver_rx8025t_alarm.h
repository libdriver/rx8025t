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
 * @file      driver_rx8025t_alarm.h
 * @brief     driver rx8025t alarm header file
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
  
#ifndef DRIVER_RX8025T_ALARM_H
#define DRIVER_RX8025T_ALARM_H

#include "driver_rx8025t_interface.h"
#include <time.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @addtogroup rx8025t_example_driver
 * @{
 */

/**
 * @brief rx8025t alarm example default definition
 */
#define RX8025T_ALARM_DEFAULT_TEMPERATURE_COMPENSATION_INTERVAL        RX8025T_TEMPERATURE_COMPENSATION_INTERVAL_0P5_SECOND        /**< 0.5s */
#define RX8025T_ALARM_DEFAULT_INTERRUPT_UPDATE                         RX8025T_BOOL_FALSE                                          /**< disable */
#define RX8025T_ALARM_DEFAULT_UPDATE_SELECT                            RX8025T_UPDATE_SELECT_SECOND                                /**< second update */

/**
 * @brief  alarm irq
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t rx8025t_alarm_irq_handler(void);

/**
 * @brief     alarm example init
 * @param[in] *callback pointer to a callback function address
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t rx8025t_alarm_init(void (*callback)(uint8_t type));

/**
 * @brief  alarm example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t rx8025t_alarm_deinit(void);

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
                             rx8025t_bool_t minute_mask, rx8025t_bool_t hour_mask, rx8025t_bool_t day_week_mask);

/**
 * @brief     alarm example set the time
 * @param[in] *t pointer to a time structure
 * @return    status code
 *            - 0 success
 *            - 1 set time failed
 * @note      none
 */
uint8_t rx8025t_alarm_set_time(rx8025t_time_t *t);

/**
 * @brief     alarm example set the time by a unix timestamp
 * @param[in] timestamp unix timestamp
 * @return    status code
 *            - 0 success
 *            - 1 set timestamp failed
 * @note      none
 */
uint8_t rx8025t_alarm_set_timestamp(time_t timestamp);

/**
 * @brief     alarm example set the local time zone
 * @param[in] zone local time zone
 * @return    status code
 *            - 0 success
 * @note      none
 */
uint8_t rx8025t_alarm_set_timestamp_time_zone(int8_t zone);

/**
 * @brief      alarm example get the time
 * @param[out] *t pointer to a time structure
 * @return     status code
 *             - 0 success
 *             - 1 get time failed
 * @note       none
 */
uint8_t rx8025t_alarm_get_time(rx8025t_time_t *t);

/**
 * @brief      alarm example get the time in a unix timestamp
 * @param[out] *timestamp pointer to a unix timestamp buffer
 * @return     status code
 *             - 0 success
 *             - 1 get timestamp failed
 * @note       none
 */
uint8_t rx8025t_alarm_get_timestamp(time_t *timestamp);

/**
 * @brief      alarm example get the local time zone
 * @param[out] *zone pointer to a local time zone buffer
 * @return     status code
 *             - 0 success
 * @note       none
 */
uint8_t rx8025t_alarm_get_timestamp_time_zone(int8_t *zone);

/**
 * @brief      alarm example get the ascii time
 * @param[out] *buf pointer to an ascii buffer
 * @param[in]  len data length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t rx8025t_alarm_get_ascii_time(char *buf, uint8_t len);

/**
 * @brief      alarm example read ram
 * @param[out] *data pointer to a ram data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read ram failed
 * @note       none
 */
uint8_t rx8025t_alarm_read_ram(uint8_t *data);

/**
 * @brief     alarm example write ram
 * @param[in] data set data
 * @return    status code
 *            - 0 success
 *            - 1 write ram failed
 * @note      none
 */
uint8_t rx8025t_alarm_write_ram(uint8_t data);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
