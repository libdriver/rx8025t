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
 * @file      driver_rx8025t.h
 * @brief     driver rx8025t header file
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

#ifndef DRIVER_RX8025T_H
#define DRIVER_RX8025T_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup rx8025t_driver rx8025t driver function
 * @brief    rx8025t driver modules
 * @{
 */

/**
 * @addtogroup rx8025t_base_driver
 * @{
 */

/**
 * @brief rx8025t bool enumeration definition
 */
typedef enum
{
    RX8025T_BOOL_FALSE = 0x00,        /**< disable function */
    RX8025T_BOOL_TRUE  = 0x01,        /**< enable function */
} rx8025t_bool_t;

/**
 * @brief rx8025t temperature compensation interval enumeration definition
 */
typedef enum
{
    RX8025T_TEMPERATURE_COMPENSATION_INTERVAL_0P5_SECOND = 0x00,        /**< 0.5s */
    RX8025T_TEMPERATURE_COMPENSATION_INTERVAL_2P0_SECOND = 0x01,        /**< 2.0s */
    RX8025T_TEMPERATURE_COMPENSATION_INTERVAL_10_SECOND  = 0x02,        /**< 10s */
    RX8025T_TEMPERATURE_COMPENSATION_INTERVAL_30_SECOND  = 0x03,        /**< 30s */
} rx8025t_temperature_compensation_interval_t;

/**
 * @brief rx8025t flag enumeration definition
 */
typedef enum
{
    RX8025T_FLAG_UPDATE            = (1 << 5),        /**< update flag */
    RX8025T_FLAG_TIMER             = (1 << 4),        /**< timer flag */
    RX8025T_FLAG_ALARM             = (1 << 3),        /**< alarm flag */
    RX8025T_FLAG_VOLTAGE_LOW       = (1 << 1),        /**< voltage low flag */
    RX8025T_FLAG_VOLTAGE_DETECTION = (1 << 0),        /**< voltage detection flag */
} rx8025t_flag_t;

/**
 * @brief rx8025t alarm enumeration definition
 */
typedef enum
{
    RX8025T_ALARM_WEEK = 0,        /**< week */
    RX8025T_ALARM_DAY  = 1,        /**< day */
} rx8025t_alarm_t;

/**
 * @brief rx8025t update select enumeration definition
 */
typedef enum
{
    RX8025T_UPDATE_SELECT_SECOND = 0,        /**< second update */
    RX8025T_UPDATE_SELECT_MINUTE = 1,        /**< minute update */
} rx8025t_update_select_t;

/**
 * @brief rx8025t timer clock enumeration definition
 */
typedef enum
{
    RX8025T_TIMER_CLOCK_4096_HZ = 0x00,        /**< 4096hz */
    RX8025T_TIMER_CLOCK_64_HZ   = 0x01,        /**< 64hz */
    RX8025T_TIMER_CLOCK_1_HZ    = 0x02,        /**< 1 hz */
    RX8025T_TIMER_CLOCK_MINUTE  = 0x03,        /**< 1 minute */
} rx8025t_timer_clock_t;

/**
 * @brief rx8025t square wave frequency enumeration definition
 */
typedef enum
{
    RX8025T_SQUARE_WAVE_FREQUENCY_1_HZ     = 0x02,        /**< 1Hz */
    RX8025T_SQUARE_WAVE_FREQUENCY_1024_HZ  = 0x01,        /**< 1.024kHz */
    RX8025T_SQUARE_WAVE_FREQUENCY_32768_HZ = 0x00,        /**< 32.768kHz */
} rx8025t_square_wave_frequency_t;

/**
 * @brief rx8025t status enumeration definition
 */
typedef enum
{
    RX8025T_STATUS_UPDATE            = 0x00,        /**< update */
    RX8025T_STATUS_TIMER             = 0x10,        /**< timer */
    RX8025T_STATUS_ALARM             = 0x20,        /**< alarm */
    RX8025T_STATUS_VOLTAGE_LOW       = 0x30,        /**< voltage low */
    RX8025T_STATUS_VOLTAGE_DETECTION = 0x40,        /**< voltage detection */
} rx8025t_status_t;

/**
 * @brief rx8025t time structure definition
 */
typedef struct rx8025t_time_s
{
    uint16_t year;                 /**< year */
    uint8_t month;                 /**< month */
    uint8_t week;                  /**< week */
    uint8_t date;                  /**< date */
    uint8_t hour;                  /**< hour */
    uint8_t minute;                /**< minute */
    uint8_t second;                /**< second */
} rx8025t_time_t;

/**
 * @brief rx8025t handle structure definition
 */
typedef struct rx8025t_handle_s
{
    uint8_t (*iic_init)(void);                                                          /**< point to an iic_init function address */
    uint8_t (*iic_deinit)(void);                                                        /**< point to an iic_deinit function address */
    uint8_t (*iic_write)(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len);        /**< point to an iic_write function address */
    uint8_t (*iic_read)(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len);         /**< point to an iic_read function address */
    void (*receive_callback)(uint8_t type);                                             /**< point to a receive_callback function address */
    void (*debug_print)(const char *const fmt, ...);                                    /**< point to a debug_print function address */
    void (*delay_ms)(uint32_t ms);                                                      /**< point to a delay_ms function address */
    uint8_t inited;                                                                     /**< inited flag */
} rx8025t_handle_t;

/**
 * @brief rx8025t information structure definition
 */
typedef struct rx8025t_info_s
{
    char chip_name[32];                /**< chip name */
    char manufacturer_name[32];        /**< manufacturer name */
    char interface[8];                 /**< chip interface name */
    float supply_voltage_min_v;        /**< chip min supply voltage */
    float supply_voltage_max_v;        /**< chip max supply voltage */
    float max_current_ma;              /**< chip max current */
    float temperature_min;             /**< chip min operating temperature */
    float temperature_max;             /**< chip max operating temperature */
    uint32_t driver_version;           /**< driver version */
} rx8025t_info_t;

/**
 * @}
 */

/**
 * @defgroup rx8025t_link_driver rx8025t link driver function
 * @brief    rx8025t link driver modules
 * @ingroup  rx8025t_driver
 * @{
 */

/**
 * @brief     initialize rx8025t_handle_t structure
 * @param[in] HANDLE pointer to a rx8025t handle structure
 * @param[in] STRUCTURE rx8025t_handle_t
 * @note      none
 */
#define DRIVER_RX8025T_LINK_INIT(HANDLE, STRUCTURE)         memset(HANDLE, 0, sizeof(STRUCTURE))

/**
 * @brief     link iic_init function
 * @param[in] HANDLE pointer to a rx8025t handle structure
 * @param[in] FUC pointer to an iic_init function address
 * @note      none
 */
#define DRIVER_RX8025T_LINK_IIC_INIT(HANDLE, FUC)          (HANDLE)->iic_init = FUC

/**
 * @brief     link iic_deinit function
 * @param[in] HANDLE pointer to a rx8025t handle structure
 * @param[in] FUC pointer to an iic_deinit function address
 * @note      none
 */
#define DRIVER_RX8025T_LINK_IIC_DEINIT(HANDLE, FUC)        (HANDLE)->iic_deinit = FUC

/**
 * @brief     link iic_read function
 * @param[in] HANDLE pointer to a rx8025t handle structure
 * @param[in] FUC pointer to an iic_read function address
 * @note      none
 */
#define DRIVER_RX8025T_LINK_IIC_READ(HANDLE, FUC)          (HANDLE)->iic_read = FUC

/**
 * @brief     link iic_write function
 * @param[in] HANDLE pointer to a rx8025t handle structure
 * @param[in] FUC pointer to an iic_write function address
 * @note      none
 */
#define DRIVER_RX8025T_LINK_IIC_WRITE(HANDLE, FUC)         (HANDLE)->iic_write = FUC

/**
 * @brief     link delay_ms function
 * @param[in] HANDLE pointer to a rx8025t handle structure
 * @param[in] FUC pointer to a delay_ms function address
 * @note      none
 */
#define DRIVER_RX8025T_LINK_DELAY_MS(HANDLE, FUC)          (HANDLE)->delay_ms = FUC

/**
 * @brief     link debug_print function
 * @param[in] HANDLE pointer to a rx8025t handle structure
 * @param[in] FUC pointer to a debug_print function address
 * @note      none
 */
#define DRIVER_RX8025T_LINK_DEBUG_PRINT(HANDLE, FUC)       (HANDLE)->debug_print = FUC

/**
 * @brief     link receive_callback function
 * @param[in] HANDLE pointer to a rx8025t handle structure
 * @param[in] FUC pointer to a receive_callback function address
 * @note      none
 */
#define DRIVER_RX8025T_LINK_RECEIVE_CALLBACK(HANDLE, FUC)  (HANDLE)->receive_callback = FUC

/**
 * @}
 */

/**
 * @defgroup rx8025t_base_driver rx8025t base driver function
 * @brief    rx8025t base driver modules
 * @ingroup  rx8025t_driver
 * @{
 */

/**
 * @brief      get chip's information
 * @param[out] *info pointer to a rx8025t info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t rx8025t_info(rx8025t_info_t *info);

/**
 * @brief     irq handler
 * @param[in] *handle pointer to a rx8025t handle structure
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t rx8025t_irq_handler(rx8025t_handle_t *handle);

/**
 * @brief     initialize the chip
 * @param[in] *handle pointer to a rx8025t handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic initialization failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 * @note      none
 */
uint8_t rx8025t_init(rx8025t_handle_t *handle);

/**
 * @brief     close the chip
 * @param[in] *handle pointer to a rx8025t handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic deinit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t rx8025t_deinit(rx8025t_handle_t *handle);

/**
 * @brief     start
 * @param[in] *handle pointer to a rx8025t handle structure
 * @return    status code
 *            - 0 success
 *            - 1 start failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t rx8025t_start(rx8025t_handle_t *handle);

/**
 * @brief     stop
 * @param[in] *handle pointer to a rx8025t handle structure
 * @return    status code
 *            - 0 success
 *            - 1 stop failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t rx8025t_stop(rx8025t_handle_t *handle);

/**
 * @brief     set the current time
 * @param[in] *handle pointer to a rx8025t handle structure
 * @param[in] *t pointer to a time structure
 * @return    status code
 *            - 0 success
 *            - 1 set time failed
 *            - 2 handle or time is NULL
 *            - 3 handle is not initialized
 *            - 4 time is invalid
 * @note      none
 */
uint8_t rx8025t_set_time(rx8025t_handle_t *handle, rx8025t_time_t *t);

/**
 * @brief      get the current time
 * @param[in]  *handle pointer to a rx8025t handle structure
 * @param[out] *t pointer to a time structure
 * @return     status code
 *             - 0 success
 *             - 1 get time failed
 *             - 2 handle or time is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t rx8025t_get_time(rx8025t_handle_t *handle, rx8025t_time_t *t);

/**
 * @brief     set the temperature compensation interval
 * @param[in] *handle pointer to a rx8025t handle structure
 * @param[in] interval temperature compensation interval
 * @return    status code
 *            - 0 success
 *            - 1 set temperature compensation interval failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t rx8025t_set_temperature_compensation_interval(rx8025t_handle_t *handle, rx8025t_temperature_compensation_interval_t interval);

/**
 * @brief      get the temperature compensation interval
 * @param[in]  *handle pointer to a rx8025t handle structure
 * @param[out] *interval pointer to a temperature compensation interval buffer
 * @return     status code
 *             - 0 success
 *             - 1 get temperature compensation interval failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t rx8025t_get_temperature_compensation_interval(rx8025t_handle_t *handle, rx8025t_temperature_compensation_interval_t *interval);

/**
 * @brief     enable or disable update interrupt
 * @param[in] *handle pointer to a rx8025t handle structure
 * @param[in] enable bool value
 * @return    status code
 *            - 0 success
 *            - 1 set update interrupt failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t rx8025t_set_update_interrupt(rx8025t_handle_t *handle, rx8025t_bool_t enable);

/**
 * @brief      get update interrupt status
 * @param[in]  *handle pointer to a rx8025t handle structure
 * @param[out] *enable pointer to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get update interrupt failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t rx8025t_get_update_interrupt(rx8025t_handle_t *handle, rx8025t_bool_t *enable);

/**
 * @brief     enable or disable timer interrupt
 * @param[in] *handle pointer to a rx8025t handle structure
 * @param[in] enable bool value
 * @return    status code
 *            - 0 success
 *            - 1 set timer interrupt failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t rx8025t_set_timer_interrupt(rx8025t_handle_t *handle, rx8025t_bool_t enable);

/**
 * @brief      get timer interrupt status
 * @param[in]  *handle pointer to a rx8025t handle structure
 * @param[out] *enable pointer to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get timer interrupt failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t rx8025t_get_timer_interrupt(rx8025t_handle_t *handle, rx8025t_bool_t *enable);

/**
 * @brief     enable or disable alarm interrupt
 * @param[in] *handle pointer to a rx8025t handle structure
 * @param[in] enable bool value
 * @return    status code
 *            - 0 success
 *            - 1 set alarm interrupt failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t rx8025t_set_alarm_interrupt(rx8025t_handle_t *handle, rx8025t_bool_t enable);

/**
 * @brief      get alarm interrupt status
 * @param[in]  *handle pointer to a rx8025t handle structure
 * @param[out] *enable pointer to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get alarm interrupt failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t rx8025t_get_alarm_interrupt(rx8025t_handle_t *handle, rx8025t_bool_t *enable);

/**
 * @brief      get flag
 * @param[in]  *handle pointer to a rx8025t handle structure
 * @param[out] *flag pointer to a flag buffer
 * @return     status code
 *             - 0 success
 *             - 1 get flag failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t rx8025t_get_flag(rx8025t_handle_t *handle, uint8_t *flag);

/**
 * @brief     clear flag
 * @param[in] *handle pointer to a rx8025t handle structure
 * @return    status code
 *            - 0 success
 *            - 1 clear flag failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t rx8025t_clear_flag(rx8025t_handle_t *handle);

/**
 * @brief     enable or disable test
 * @param[in] *handle pointer to a rx8025t handle structure
 * @param[in] enable bool value
 * @return    status code
 *            - 0 success
 *            - 1 set test failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t rx8025t_set_test(rx8025t_handle_t *handle, rx8025t_bool_t enable);

/**
 * @brief      get test status
 * @param[in]  *handle pointer to a rx8025t handle structure
 * @param[out] *enable pointer to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get test failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t rx8025t_get_test(rx8025t_handle_t *handle, rx8025t_bool_t *enable);

/**
 * @brief     set alarm day week
 * @param[in] *handle pointer to a rx8025t handle structure
 * @param[in] alarm alarm type
 * @return    status code
 *            - 0 success
 *            - 1 set alarm day week failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t rx8025t_set_alarm_day_week(rx8025t_handle_t *handle, rx8025t_alarm_t alarm);

/**
 * @brief      get alarm day week
 * @param[in]  *handle pointer to a rx8025t handle structure
 * @param[out] *alarm pointer to an alarm type buffer
 * @return     status code
 *             - 0 success
 *             - 1 get alarm day week failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t rx8025t_get_alarm_day_week(rx8025t_handle_t *handle, rx8025t_alarm_t *alarm);

/**
 * @brief     set update select
 * @param[in] *handle pointer to a rx8025t handle structure
 * @param[in] select update select
 * @return    status code
 *            - 0 success
 *            - 1 set update select failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t rx8025t_set_update_select(rx8025t_handle_t *handle, rx8025t_update_select_t select);

/**
 * @brief      get update select
 * @param[in]  *handle pointer to a rx8025t handle structure
 * @param[out] *select pointer to an update select buffer
 * @return     status code
 *             - 0 success
 *             - 1 get update select failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t rx8025t_get_update_select(rx8025t_handle_t *handle, rx8025t_update_select_t *select);

/**
 * @brief     enable or disable timer
 * @param[in] *handle pointer to a rx8025t handle structure
 * @param[in] enable bool value
 * @return    status code
 *            - 0 success
 *            - 1 set timer failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t rx8025t_set_timer(rx8025t_handle_t *handle, rx8025t_bool_t enable);

/**
 * @brief      get timer status
 * @param[in]  *handle pointer to a rx8025t handle structure
 * @param[out] *enable pointer to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get timer failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t rx8025t_get_timer(rx8025t_handle_t *handle, rx8025t_bool_t *enable);

/**
 * @brief     set square wave frequency
 * @param[in] *handle pointer to a rx8025t handle structure
 * @param[in] freq square wave frequency
 * @return    status code
 *            - 0 success
 *            - 1 set square wave frequency failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t rx8025t_set_square_wave_frequency(rx8025t_handle_t *handle, rx8025t_square_wave_frequency_t freq);

/**
 * @brief      get square wave frequency
 * @param[in]  *handle pointer to a rx8025t handle structure
 * @param[out] *freq pointer to a square wave frequency buffer
 * @return     status code
 *             - 0 success
 *             - 1 get square wave frequency failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t rx8025t_get_square_wave_frequency(rx8025t_handle_t *handle, rx8025t_square_wave_frequency_t *freq);

/**
 * @brief     set timer clock
 * @param[in] *handle pointer to a rx8025t handle structure
 * @param[in] clk timer clock
 * @return    status code
 *            - 0 success
 *            - 1 set timer clock failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t rx8025t_set_timer_clock(rx8025t_handle_t *handle, rx8025t_timer_clock_t clk);

/**
 * @brief      get timer clock
 * @param[in]  *handle pointer to a rx8025t handle structure
 * @param[out] *clk pointer to a timer clock buffer
 * @return     status code
 *             - 0 success
 *             - 1 get timer clock failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t rx8025t_get_timer_clock(rx8025t_handle_t *handle, rx8025t_timer_clock_t *clk);

/**
 * @brief     set timer counter
 * @param[in] *handle pointer to a rx8025t handle structure
 * @param[in] counter timer counter
 * @return    status code
 *            - 0 success
 *            - 1 set timer counter failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 counter > 0x0FFF
 * @note      none
 */
uint8_t rx8025t_set_timer_counter(rx8025t_handle_t *handle, uint16_t counter);

/**
 * @brief      get timer counter
 * @param[in]  *handle pointer to a rx8025t handle structure
 * @param[out] *counter pointer to a timer counter buffer
 * @return     status code
 *             - 0 success
 *             - 1 get timer counter failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t rx8025t_get_timer_counter(rx8025t_handle_t *handle, uint16_t *counter);

/**
 * @brief     set ram
 * @param[in] *handle pointer to a rx8025t handle structure
 * @param[in] data ram data
 * @return    status code
 *            - 0 success
 *            - 1 set ram failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t rx8025t_set_ram(rx8025t_handle_t *handle, uint8_t data);

/**
 * @brief      get ram
 * @param[in]  *handle pointer to a rx8025t handle structure
 * @param[out] *data pointer to a ram data buffer
 * @return     status code
 *             - 0 success
 *             - 1 get ram failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t rx8025t_get_ram(rx8025t_handle_t *handle, uint8_t *data);

/**
 * @brief     set minute alarm
 * @param[in] *handle pointer to a rx8025t handle structure
 * @param[in] minute input minute
 * @return    status code
 *            - 0 success
 *            - 1 set minute alarm failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 minute can't be over than 59
 * @note      none
 */
uint8_t rx8025t_set_minute_alarm(rx8025t_handle_t *handle, uint8_t minute);

/**
 * @brief      get minute alarm
 * @param[in]  *handle pointer to a rx8025t handle structure
 * @param[out] *minute pointer to a minute buffer
 * @return     status code
 *             - 0 success
 *             - 1 get minute alarm failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t rx8025t_get_minute_alarm(rx8025t_handle_t *handle, uint8_t *minute);

/**
 * @brief     set hour alarm
 * @param[in] *handle pointer to a rx8025t handle structure
 * @param[in] hour input hour
 * @return    status code
 *            - 0 success
 *            - 1 set hour alarm failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 hour can't be over than 23
 * @note      none
 */
uint8_t rx8025t_set_hour_alarm(rx8025t_handle_t *handle, uint8_t hour);

/**
 * @brief      get hour alarm
 * @param[in]  *handle pointer to a rx8025t handle structure
 * @param[out] *hour pointer to an hour buffer
 * @return     status code
 *             - 0 success
 *             - 1 get hour alarm failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t rx8025t_get_hour_alarm(rx8025t_handle_t *handle, uint8_t *hour);

/**
 * @brief     set date alarm
 * @param[in] *handle pointer to a rx8025t handle structure
 * @param[in] date input date
 * @return    status code
 *            - 0 success
 *            - 1 set date alarm failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 date can't be zero or over than 31
 * @note      none
 */
uint8_t rx8025t_set_date_alarm(rx8025t_handle_t *handle, uint8_t date);

/**
 * @brief      get date alarm
 * @param[in]  *handle pointer to a rx8025t handle structure
 * @param[out] *date pointer to a date buffer
 * @return     status code
 *             - 0 success
 *             - 1 get date alarm failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t rx8025t_get_date_alarm(rx8025t_handle_t *handle, uint8_t *date);

/**
 * @brief     set week alarm
 * @param[in] *handle pointer to a rx8025t handle structure
 * @param[in] week input week
 * @return    status code
 *            - 0 success
 *            - 1 set week alarm failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 week can't be zero or over than 7
 * @note      none
 */
uint8_t rx8025t_set_week_alarm(rx8025t_handle_t *handle, uint8_t week);

/**
 * @brief      get week alarm
 * @param[in]  *handle pointer to a rx8025t handle structure
 * @param[out] *week pointer to a week buffer
 * @return     status code
 *             - 0 success
 *             - 1 get week alarm failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t rx8025t_get_week_alarm(rx8025t_handle_t *handle, uint8_t *week);

/**
 * @brief     enable or disable minute alarm mask
 * @param[in] *handle pointer to a rx8025t handle structure
 * @param[in] enable bool value
 * @return    status code
 *            - 0 success
 *            - 1 set minute alarm mask failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t rx8025t_set_minute_alarm_mask(rx8025t_handle_t *handle, rx8025t_bool_t enable);

/**
 * @brief      get minute alarm mask status
 * @param[in]  *handle pointer to a rx8025t handle structure
 * @param[out] *enable pointer to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get minute alarm mask failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t rx8025t_get_minute_alarm_mask(rx8025t_handle_t *handle, rx8025t_bool_t *enable);

/**
 * @brief     enable or disable hour alarm mask
 * @param[in] *handle pointer to a rx8025t handle structure
 * @param[in] enable bool value
 * @return    status code
 *            - 0 success
 *            - 1 set hour alarm mask failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t rx8025t_set_hour_alarm_mask(rx8025t_handle_t *handle, rx8025t_bool_t enable);

/**
 * @brief      get hour alarm mask status
 * @param[in]  *handle pointer to a rx8025t handle structure
 * @param[out] *enable pointer to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get hour alarm mask failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t rx8025t_get_hour_alarm_mask(rx8025t_handle_t *handle, rx8025t_bool_t *enable);

/**
 * @brief     enable or disable week day alarm mask
 * @param[in] *handle pointer to a rx8025t handle structure
 * @param[in] enable bool value
 * @return    status code
 *            - 0 success
 *            - 1 set week day alarm mask failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t rx8025t_set_week_day_alarm_mask(rx8025t_handle_t *handle, rx8025t_bool_t enable);

/**
 * @brief      get week day alarm mask status
 * @param[in]  *handle pointer to a rx8025t handle structure
 * @param[out] *enable pointer to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get week day alarm mask failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t rx8025t_get_week_day_alarm_mask(rx8025t_handle_t *handle, rx8025t_bool_t *enable);

/**
 * @}
 */

/**
 * @defgroup rx8025t_extern_driver rx8025t extern driver function
 * @brief    rx8025t extern driver modules
 * @ingroup  rx8025t_driver
 * @{
 */

/**
 * @brief     set the chip register
 * @param[in] *handle pointer to a rx8025t handle structure
 * @param[in] reg iic register address
 * @param[in] *buf pointer to a data buffer
 * @param[in] len data buffer length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t rx8025t_set_reg(rx8025t_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len);

/**
 * @brief      get the chip register
 * @param[in]  *handle pointer to a rx8025t handle structure
 * @param[in]  reg iic register address
 * @param[out] *buf pointer to a data buffer
 * @param[in]  len data buffer length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t rx8025t_get_reg(rx8025t_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len);

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
