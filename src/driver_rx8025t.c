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
 * @file      driver_rx8025t.c
 * @brief     driver rx8025t source file
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

#include "driver_rx8025t.h"

/**
 * @brief chip information definition
 */
#define CHIP_NAME                 "EPSON RX8025T"        /**< chip name */
#define MANUFACTURER_NAME         "EPSON"                /**< manufacturer name */
#define SUPPLY_VOLTAGE_MIN        1.6f                   /**< chip min supply voltage */
#define SUPPLY_VOLTAGE_MAX        5.5f                   /**< chip max supply voltage */
#define MAX_CURRENT               0.90f                  /**< chip max current */
#define TEMPERATURE_MIN           -40.0f                 /**< chip min operating temperature */
#define TEMPERATURE_MAX           85.0f                  /**< chip max operating temperature */
#define DRIVER_VERSION            1000                   /**< driver version */

/**
 * @brief chip register definition
 */
#define RX8025T_REG_SEC                    0x00        /**< second register */
#define RX8025T_REG_MIN                    0x01        /**< minute register */
#define RX8025T_REG_HOUR                   0x02        /**< hour register */
#define RX8025T_REG_WEEK                   0x03        /**< week register */
#define RX8025T_REG_DAY                    0x04        /**< date register */
#define RX8025T_REG_MONTH                  0x05        /**< month register */
#define RX8025T_REG_YEAR                   0x06        /**< year register */
#define RX8025T_REG_RAM                    0x07        /**< ram register */
#define RX8025T_REG_MIN_ALARM              0x08        /**< min alarm register */
#define RX8025T_REG_HOUR_ALARM             0x09        /**< hour alarm register */
#define RX8025T_REG_WEEK_DAY_ALARM         0x0A        /**< week day alarm register */
#define RX8025T_REG_TIMER_COUNTER_0        0x0B        /**< timer counter 0 register */
#define RX8025T_REG_TIMER_COUNTER_1        0x0C        /**< timer counter 1 register */
#define RX8025T_REG_EXTENSION              0x0D        /**< extension register */
#define RX8025T_REG_FLAG                   0x0E        /**< flag register */
#define RX8025T_REG_CONTROL                0x0F        /**< control register */

/**
 * @brief chip address definition
 */
#define RX8025T_ADDRESS        0x64        /**< iic device address */ 

/**
 * @brief     write one byte
 * @param[in] *handle pointer to a rx8025t handle structure
 * @param[in] reg iic register address
 * @param[in] data written data
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
static uint8_t a_rx8025t_iic_write(rx8025t_handle_t *handle, uint8_t reg, uint8_t data)
{
    if (handle->iic_write(RX8025T_ADDRESS, reg, &data, 1) != 0)        /* write data */
    {
        return 1;                                                      /* return error */
    }
    
    return 0;                                                          /* success return 0 */
}

/**
 * @brief     write multiple bytes
 * @param[in] *handle pointer to a rx8025t handle structure
 * @param[in] reg iic register address
 * @param[in] *buf pointer to a data buffer
 * @param[in] len data buffer length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
static uint8_t a_rx8025t_iic_multiple_write(rx8025t_handle_t *handle, uint8_t reg, uint8_t *buf, uint8_t len)
{
    if (handle->iic_write(RX8025T_ADDRESS, reg, buf, len) != 0)        /* write data */
    {
        return 1;                                                      /* return error */
    }
    
    return 0;                                                          /* success return 0 */
}

/**
 * @brief      read multiple bytes
 * @param[in]  *handle pointer to a rx8025t handle structure
 * @param[in]  reg iic register address
 * @param[out] *buf pointer to a data buffer
 * @param[in]  len data buffer length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
static uint8_t a_rx8025t_iic_multiple_read(rx8025t_handle_t *handle, uint8_t reg, uint8_t *buf, uint8_t len)
{
    if (handle->iic_read(RX8025T_ADDRESS, reg, buf, len) != 0)        /* read data */
    {
        return 1;                                                     /* return error */
    }
    
    return 0;                                                         /* success return 0 */
}

/**
 * @brief     hex to bcd
 * @param[in] val hex data
 * @return    bcd data
 * @note      none
 */
static uint8_t a_rx8025t_hex2bcd(uint8_t val)
{
    uint8_t i, j, k;
    
    i = val / 10;            /* get tens place */
    j = val % 10;            /* get ones place */
    k = j + (i << 4);        /* set bcd */
    
    return k;                /* return bcd */
}

/**
 * @brief     bcd to hex
 * @param[in] val bcd data
 * @return    hex data
 * @note      none
 */
static uint8_t a_rx8025t_bcd2hex(uint8_t val)
{
    uint8_t temp;
    
    temp = val & 0x0F;              /* get ones place */
    val = (val >> 4) & 0x0F;        /* get tens place */
    val = val * 10;                 /* set tens place */
    temp = temp + val;              /* get hex */
    
    return temp;                    /* return hex */
}

/**
 * @brief     week convert
 * @param[in] data input data
 * @return    index
 * @note      none
 */
static uint8_t a_week_convert(uint8_t data)
{
    uint8_t i;
    
    for (i = 0; i < 8; i++)                  /* 8 bits */
    {
        if (((data >> i) & 0x1) != 0)        /* check 1 */
        {
            break;                           /* break */
        }
    }
    
    return i;                                /* return the index */
}

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
uint8_t rx8025t_set_temperature_compensation_interval(rx8025t_handle_t *handle, rx8025t_temperature_compensation_interval_t interval)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_rx8025t_iic_multiple_read(handle, RX8025T_REG_CONTROL, &prev, 1);        /* read control */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("rx8025t: read control failed.\n");                      /* read control failed */
        
        return 1;                                                                    /* return error */
    }
    prev &= ~(3 << 6);                                                               /* clear config */
    prev |= interval << 6;                                                           /* set interval */
    res = a_rx8025t_iic_write(handle, RX8025T_REG_CONTROL, prev);                    /* write control */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("rx8025t: write control failed.\n");                     /* write control failed */
        
        return 1;                                                                    /* return error */
    }
    
    return 0;                                                                        /* success return 0 */
}

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
uint8_t rx8025t_get_temperature_compensation_interval(rx8025t_handle_t *handle, rx8025t_temperature_compensation_interval_t *interval)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_rx8025t_iic_multiple_read(handle, RX8025T_REG_CONTROL, &prev, 1);             /* read control */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("rx8025t: read control failed.\n");                           /* read control failed */
        
        return 1;                                                                         /* return error */
    }
    *interval = (rx8025t_temperature_compensation_interval_t)((prev >> 6) & 0x03);        /* set the interval */
    
    return 0;                                                                             /* success return 0 */
}

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
uint8_t rx8025t_set_update_interrupt(rx8025t_handle_t *handle, rx8025t_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_rx8025t_iic_multiple_read(handle, RX8025T_REG_CONTROL, &prev, 1);        /* read control */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("rx8025t: read control failed.\n");                      /* read control failed */
        
        return 1;                                                                    /* return error */
    }
    prev &= ~(1 << 5);                                                               /* clear config */
    prev |= enable << 5;                                                             /* set bool */
    res = a_rx8025t_iic_write(handle, RX8025T_REG_CONTROL, prev);                    /* write control */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("rx8025t: write control failed.\n");                     /* write control failed */
        
        return 1;                                                                    /* return error */
    }
    
    return 0;                                                                        /* success return 0 */
}

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
uint8_t rx8025t_get_update_interrupt(rx8025t_handle_t *handle, rx8025t_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_rx8025t_iic_multiple_read(handle, RX8025T_REG_CONTROL, &prev, 1);        /* read control */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("rx8025t: read control failed.\n");                      /* read control failed */
        
        return 1;                                                                    /* return error */
    }
    *enable = (rx8025t_bool_t)((prev >> 5) & 0x01);                                  /* set bool */
    
    return 0;                                                                        /* success return 0 */
}

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
uint8_t rx8025t_set_timer_interrupt(rx8025t_handle_t *handle, rx8025t_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_rx8025t_iic_multiple_read(handle, RX8025T_REG_CONTROL, &prev, 1);        /* read control */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("rx8025t: read control failed.\n");                      /* read control failed */
        
        return 1;                                                                    /* return error */
    }
    prev &= ~(1 << 4);                                                               /* clear config */
    prev |= enable << 4;                                                             /* set bool */
    res = a_rx8025t_iic_write(handle, RX8025T_REG_CONTROL, prev);                    /* write control */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("rx8025t: write control failed.\n");                     /* write control failed */
        
        return 1;                                                                    /* return error */
    }
    
    return 0;                                                                        /* success return 0 */
}

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
uint8_t rx8025t_get_timer_interrupt(rx8025t_handle_t *handle, rx8025t_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_rx8025t_iic_multiple_read(handle, RX8025T_REG_CONTROL, &prev, 1);        /* read control */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("rx8025t: read control failed.\n");                      /* read control failed */
        
        return 1;                                                                    /* return error */
    }
    *enable = (rx8025t_bool_t)((prev >> 4) & 0x01);                                  /* set bool */
    
    return 0;                                                                        /* success return 0 */
}

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
uint8_t rx8025t_set_alarm_interrupt(rx8025t_handle_t *handle, rx8025t_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_rx8025t_iic_multiple_read(handle, RX8025T_REG_CONTROL, &prev, 1);        /* read control */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("rx8025t: read control failed.\n");                      /* read control failed */
        
        return 1;                                                                    /* return error */
    }
    prev &= ~(1 << 3);                                                               /* clear config */
    prev |= enable << 3;                                                             /* set bool */
    res = a_rx8025t_iic_write(handle, RX8025T_REG_CONTROL, prev);                    /* write control */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("rx8025t: write control failed.\n");                     /* write control failed */
        
        return 1;                                                                    /* return error */
    }
    
    return 0;                                                                        /* success return 0 */
}

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
uint8_t rx8025t_get_alarm_interrupt(rx8025t_handle_t *handle, rx8025t_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_rx8025t_iic_multiple_read(handle, RX8025T_REG_CONTROL, &prev, 1);        /* read control */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("rx8025t: read control failed.\n");                      /* read control failed */
        
        return 1;                                                                    /* return error */
    }
    *enable = (rx8025t_bool_t)((prev >> 3) & 0x01);                                  /* set bool */
    
    return 0;                                                                        /* success return 0 */
}

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
uint8_t rx8025t_stop(rx8025t_handle_t *handle)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_rx8025t_iic_multiple_read(handle, RX8025T_REG_CONTROL, &prev, 1);        /* read control */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("rx8025t: read control failed.\n");                      /* read control failed */
        
        return 1;                                                                    /* return error */
    }
    prev &= ~(1 << 0);                                                               /* clear config */
    prev |= 1 << 0;                                                                  /* set bool */
    res = a_rx8025t_iic_write(handle, RX8025T_REG_CONTROL, prev);                    /* write control */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("rx8025t: write control failed.\n");                     /* write control failed */
        
        return 1;                                                                    /* return error */
    }
    
    return 0;                                                                        /* success return 0 */
}

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
uint8_t rx8025t_start(rx8025t_handle_t *handle)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_rx8025t_iic_multiple_read(handle, RX8025T_REG_CONTROL, &prev, 1);        /* read control */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("rx8025t: read control failed.\n");                      /* read control failed */
        
        return 1;                                                                    /* return error */
    }
    prev &= ~(1 << 0);                                                               /* clear config */
    res = a_rx8025t_iic_write(handle, RX8025T_REG_CONTROL, prev);                    /* write control */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("rx8025t: write control failed.\n");                     /* write control failed */
        
        return 1;                                                                    /* return error */
    }
    
    return 0;                                                                        /* success return 0 */
}

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
uint8_t rx8025t_get_flag(rx8025t_handle_t *handle, uint8_t *flag)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                           /* check handle */
    {
        return 2;                                                                 /* return error */
    }
    if (handle->inited != 1)                                                      /* check handle initialization */
    {
        return 3;                                                                 /* return error */
    }
    
    res = a_rx8025t_iic_multiple_read(handle, RX8025T_REG_FLAG, &prev, 1);        /* read flag */
    if (res != 0)                                                                 /* check result */
    {
        handle->debug_print("rx8025t: read flag failed.\n");                      /* read flag failed */
        
        return 1;                                                                 /* return error */
    }
    *flag = prev;                                                                 /* set flag */
    
    return 0;                                                                     /* success return 0 */
}

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
uint8_t rx8025t_set_test(rx8025t_handle_t *handle, rx8025t_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_rx8025t_iic_multiple_read(handle, RX8025T_REG_EXTENSION, &prev, 1);      /* read extension */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("rx8025t: read extension failed.\n");                    /* read extension failed */
        
        return 1;                                                                    /* return error */
    }
    prev &= ~(1 << 7);                                                               /* clear config */
    prev |= enable << 7;                                                             /* set bool */
    res = a_rx8025t_iic_write(handle, RX8025T_REG_EXTENSION, prev);                  /* write extension */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("rx8025t: write extension failed.\n");                   /* write extension failed */
        
        return 1;                                                                    /* return error */
    }
    
    return 0;                                                                        /* success return 0 */
}

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
uint8_t rx8025t_get_test(rx8025t_handle_t *handle, rx8025t_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_rx8025t_iic_multiple_read(handle, RX8025T_REG_EXTENSION, &prev, 1);      /* read extension */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("rx8025t: read extension failed.\n");                    /* read extension failed */
        
        return 1;                                                                    /* return error */
    }
    *enable = (rx8025t_bool_t)((prev >> 7) & 0x01);                                  /* set bool */
    
    return 0;                                                                        /* success return 0 */
}

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
uint8_t rx8025t_set_alarm_day_week(rx8025t_handle_t *handle, rx8025t_alarm_t alarm)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_rx8025t_iic_multiple_read(handle, RX8025T_REG_EXTENSION, &prev, 1);      /* read extension */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("rx8025t: read extension failed.\n");                    /* read extension failed */
        
        return 1;                                                                    /* return error */
    }
    prev &= ~(1 << 6);                                                               /* clear config */
    prev |= alarm << 6;                                                              /* set alarm */
    res = a_rx8025t_iic_write(handle, RX8025T_REG_EXTENSION, prev);                  /* write extension */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("rx8025t: write extension failed.\n");                   /* write extension failed */
        
        return 1;                                                                    /* return error */
    }
    
    return 0;                                                                        /* success return 0 */
}

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
uint8_t rx8025t_get_alarm_day_week(rx8025t_handle_t *handle, rx8025t_alarm_t *alarm)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_rx8025t_iic_multiple_read(handle, RX8025T_REG_EXTENSION, &prev, 1);      /* read extension */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("rx8025t: read extension failed.\n");                    /* read extension failed */
        
        return 1;                                                                    /* return error */
    }
    *alarm = (rx8025t_alarm_t)((prev >> 6) & 0x01);                                  /* set alarm */
    
    return 0;                                                                        /* success return 0 */
}

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
uint8_t rx8025t_set_update_select(rx8025t_handle_t *handle, rx8025t_update_select_t select)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_rx8025t_iic_multiple_read(handle, RX8025T_REG_EXTENSION, &prev, 1);      /* read extension */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("rx8025t: read extension failed.\n");                    /* read extension failed */
        
        return 1;                                                                    /* return error */
    }
    prev &= ~(1 << 5);                                                               /* clear config */
    prev |= select << 5;                                                             /* set update select */
    res = a_rx8025t_iic_write(handle, RX8025T_REG_EXTENSION, prev);                  /* write extension */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("rx8025t: write extension failed.\n");                   /* write extension failed */
        
        return 1;                                                                    /* return error */
    }
    
    return 0;                                                                        /* success return 0 */
}

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
uint8_t rx8025t_get_update_select(rx8025t_handle_t *handle, rx8025t_update_select_t *select)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_rx8025t_iic_multiple_read(handle, RX8025T_REG_EXTENSION, &prev, 1);      /* read extension */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("rx8025t: read extension failed.\n");                    /* read extension failed */
        
        return 1;                                                                    /* return error */
    }
    *select = (rx8025t_update_select_t)((prev >> 5) & 0x01);                         /* set update select */
    
    return 0;                                                                        /* success return 0 */
}

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
uint8_t rx8025t_set_timer(rx8025t_handle_t *handle, rx8025t_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_rx8025t_iic_multiple_read(handle, RX8025T_REG_EXTENSION, &prev, 1);      /* read extension */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("rx8025t: read extension failed.\n");                    /* read extension failed */
        
        return 1;                                                                    /* return error */
    }
    prev &= ~(1 << 4);                                                               /* clear config */
    prev |= enable << 4;                                                             /* set bool */
    res = a_rx8025t_iic_write(handle, RX8025T_REG_EXTENSION, prev);                  /* write extension */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("rx8025t: write extension failed.\n");                   /* write extension failed */
        
        return 1;                                                                    /* return error */
    }
    
    return 0;                                                                        /* success return 0 */
}

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
uint8_t rx8025t_get_timer(rx8025t_handle_t *handle, rx8025t_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_rx8025t_iic_multiple_read(handle, RX8025T_REG_EXTENSION, &prev, 1);      /* read extension */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("rx8025t: read extension failed.\n");                    /* read extension failed */
        
        return 1;                                                                    /* return error */
    }
    *enable = (rx8025t_bool_t)((prev >> 4) & 0x01);                                  /* set bool */
    
    return 0;                                                                        /* success return 0 */
}

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
uint8_t rx8025t_set_square_wave_frequency(rx8025t_handle_t *handle, rx8025t_square_wave_frequency_t freq)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_rx8025t_iic_multiple_read(handle, RX8025T_REG_EXTENSION, &prev, 1);      /* read extension */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("rx8025t: read extension failed.\n");                    /* read extension failed */
        
        return 1;                                                                    /* return error */
    }
    prev &= ~(3 << 2);                                                               /* clear config */
    prev |= freq << 2;                                                               /* set frequency */
    res = a_rx8025t_iic_write(handle, RX8025T_REG_EXTENSION, prev);                  /* write extension */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("rx8025t: write extension failed.\n");                   /* write extension failed */
        
        return 1;                                                                    /* return error */
    }
    
    return 0;                                                                        /* success return 0 */
}

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
uint8_t rx8025t_get_square_wave_frequency(rx8025t_handle_t *handle, rx8025t_square_wave_frequency_t *freq)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_rx8025t_iic_multiple_read(handle, RX8025T_REG_EXTENSION, &prev, 1);      /* read extension */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("rx8025t: read extension failed.\n");                    /* read extension failed */
        
        return 1;                                                                    /* return error */
    }
    *freq = (rx8025t_square_wave_frequency_t)((prev >> 2) & 0x03);                   /* set frequency */
    
    return 0;                                                                        /* success return 0 */
}

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
uint8_t rx8025t_set_timer_clock(rx8025t_handle_t *handle, rx8025t_timer_clock_t clk)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_rx8025t_iic_multiple_read(handle, RX8025T_REG_EXTENSION, &prev, 1);      /* read extension */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("rx8025t: read extension failed.\n");                    /* read extension failed */
        
        return 1;                                                                    /* return error */
    }
    prev &= ~(3 << 0);                                                               /* clear config */
    prev |= clk << 0;                                                                /* set timer clock */
    res = a_rx8025t_iic_write(handle, RX8025T_REG_EXTENSION, prev);                  /* write extension */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("rx8025t: write extension failed.\n");                   /* write extension failed */
        
        return 1;                                                                    /* return error */
    }
    
    return 0;                                                                        /* success return 0 */
}

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
uint8_t rx8025t_get_timer_clock(rx8025t_handle_t *handle, rx8025t_timer_clock_t *clk)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_rx8025t_iic_multiple_read(handle, RX8025T_REG_EXTENSION, &prev, 1);      /* read extension */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("rx8025t: read extension failed.\n");                    /* read extension failed */
        
        return 1;                                                                    /* return error */
    }
    *clk = (rx8025t_timer_clock_t)((prev >> 0) & 0x03);                              /* set clock */
    
    return 0;                                                                        /* success return 0 */
}

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
uint8_t rx8025t_set_timer_counter(rx8025t_handle_t *handle, uint16_t counter)
{
    uint8_t res;
    uint8_t buf[2];
    
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }
    if (counter > 0x0FFF)                                                                   /* check counter */
    {
        handle->debug_print("rx8025t: counter > 0x0FFF.\n");                                /* counter > 0x0FFF */
        
        return 4;                                                                           /* return error */
    }
    
    buf[0] = (counter >> 0) & 0xFF;                                                         /* set lsb */
    buf[1] = (counter >> 8) & 0xFF;                                                         /* set msb */
    res = a_rx8025t_iic_multiple_write(handle, RX8025T_REG_TIMER_COUNTER_0, buf, 2);        /* write timer counter */
    if (res != 0)                                                                           /* check result */
    {
        handle->debug_print("rx8025t: write timer counter failed.\n");                      /* write timer counter failed */
        
        return 1;                                                                           /* return error */
    }
    
    return 0;                                                                               /* success return 0 */
}

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
uint8_t rx8025t_get_timer_counter(rx8025t_handle_t *handle, uint16_t *counter)
{
    uint8_t res;
    uint8_t buf[2];
    
    if (handle == NULL)                                                                    /* check handle */
    {
        return 2;                                                                          /* return error */
    }
    if (handle->inited != 1)                                                               /* check handle initialization */
    {
        return 3;                                                                          /* return error */
    }
    
    res = a_rx8025t_iic_multiple_read(handle, RX8025T_REG_TIMER_COUNTER_0, buf, 2);        /* read timer counter */
    if (res != 0)                                                                          /* check result */
    {
        handle->debug_print("rx8025t: read timer counter failed.\n");                      /* read timer counter failed */
        
        return 1;                                                                          /* return error */
    }
    *counter = (uint16_t)(((uint16_t)(buf[1] & 0x0F)) << 8) | buf[0];                      /* set counter */
    
    return 0;                                                                              /* success return 0 */
}

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
uint8_t rx8025t_set_ram(rx8025t_handle_t *handle, uint8_t data)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                              /* check handle */
    {
        return 2;                                                    /* return error */
    }
    if (handle->inited != 1)                                         /* check handle initialization */
    {
        return 3;                                                    /* return error */
    }
    
    prev = data;                                                     /* set raw */
    res = a_rx8025t_iic_write(handle, RX8025T_REG_RAM, prev);        /* write raw */
    if (res != 0)                                                    /* check result */
    {
        handle->debug_print("rx8025t: write raw failed.\n");         /* write raw failed */
        
        return 1;                                                    /* return error */
    }
    
    return 0;                                                        /* success return 0 */
}

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
uint8_t rx8025t_get_ram(rx8025t_handle_t *handle, uint8_t *data)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                          /* check handle */
    {
        return 2;                                                                /* return error */
    }
    if (handle->inited != 1)                                                     /* check handle initialization */
    {
        return 3;                                                                /* return error */
    }
    
    res = a_rx8025t_iic_multiple_read(handle, RX8025T_REG_RAM, &prev, 1);        /* read raw */
    if (res != 0)                                                                /* check result */
    {
        handle->debug_print("rx8025t: read raw failed.\n");                      /* read raw failed */
        
        return 1;                                                                /* return error */
    }
    *data = prev;                                                                /* set data */
    
    return 0;                                                                    /* success return 0 */
}

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
uint8_t rx8025t_set_time(rx8025t_handle_t *handle, rx8025t_time_t *t)
{
    uint8_t res;
    uint8_t reg;
    uint16_t year;
    
    if (handle == NULL)                                                                                  /* check handle */
    {
        return 2;                                                                                        /* return error */
    }
    if (handle->inited != 1)                                                                             /* check handle initialization */
    {
        return 3;                                                                                        /* return error */
    }
    if (t == NULL)                                                                                       /* check time */
    {
        handle->debug_print("rx8025t: time is null.\n");                                                 /* time is null */
        
        return 2;                                                                                        /* return error */
    }
    
    if ((t->year < 2000) || (t->year > 2099))                                                            /* check year */
    {
        handle->debug_print("rx8025t: year can't be over 2099 or less than 2000.\n");                    /* year can't be over 2099 or less than 2000 */
        
        return 4;                                                                                        /* return error */
    }
    if ((t->month == 0) || (t->month > 12))                                                              /* check month */
    {
        handle->debug_print("rx8025t: month can't be zero or over than 12.\n");                          /* month can't be zero or over than 12 */
        
        return 4;                                                                                        /* return error */
    }
    if ((t->week == 0) || (t->week > 7))                                                                 /* check week */
    {
        handle->debug_print("rx8025t: week can't be zero or over than 7.\n");                            /* week can't be zero or over than 7 */
        
        return 4;                                                                                        /* return error */
    }
    if ((t->date == 0) || (t->date > 31))                                                                /* check data */
    {
        handle->debug_print("rx8025t: date can't be zero or over than 31.\n");                           /* date can't be zero or over than 31 */
        
        return 4;                                                                                        /* return error */
    }
    if (t->hour > 23)                                                                                    /* check hour */
    {
        handle->debug_print("rx8025t: hour can't be over than 23.\n");                                   /* hour can't be over than 23 */
        
        return 4;                                                                                        /* return error */
    }
    if (t->minute > 59)                                                                                  /* check minute */
    {
        handle->debug_print("rx8025t: minute can't be over than 59.\n");                                 /* minute can't be over than 59 */
        
        return 4;                                                                                        /* return error */
    }
    if (t->second > 59)                                                                                  /* check second */
    {
        handle->debug_print("rx8025t: second can't be over than 59.\n");                                 /* second can't be over than 59 */
        
        return 4;                                                                                        /* return error */
    }
    
    res = a_rx8025t_iic_write(handle, RX8025T_REG_SEC, a_rx8025t_hex2bcd(t->second));                    /* write second */
    if (res != 0)                                                                                        /* check result */
    {
        handle->debug_print("rx8025t: write second failed.\n");                                          /* write second failed */
        
        return 1;                                                                                        /* return error */
    }
    res = a_rx8025t_iic_write(handle, RX8025T_REG_MIN, a_rx8025t_hex2bcd(t->minute));                    /* write minute */
    if (res != 0)                                                                                        /* check result */
    {
        handle->debug_print("rx8025t: write minute failed.\n");                                          /* write minute failed */
        
        return 1;                                                                                        /* return error */
    }
    res = a_rx8025t_iic_write(handle, RX8025T_REG_HOUR, a_rx8025t_hex2bcd(t->hour));                     /* write hour */
    if (res != 0)                                                                                        /* check result */
    {
        handle->debug_print("rx8025t: write hour failed.\n");                                            /* write hour failed */
        
        return 1;                                                                                        /* return error */
    }
    if (t->week == 7)                                                                                    /* sunday */
    {
        reg = 0;                                                                                         /* sunday set bit 0 */
    }
    else
    {
        reg = t->week;                                                                                   /* set week bit */
    }
    res = a_rx8025t_iic_write(handle, RX8025T_REG_WEEK, 1 << reg);                                       /* write week */
    if (res != 0)                                                                                        /* check result */
    {
        handle->debug_print("rx8025t: write week failed.\n");                                            /* write week failed */
        
        return 1;                                                                                        /* return error */
    }
    res = a_rx8025t_iic_write(handle, RX8025T_REG_DAY, a_rx8025t_hex2bcd(t->date));                      /* write data */
    if (res != 0)                                                                                        /* check result */
    {
        handle->debug_print("rx8025t: write date failed.\n");                                            /* write date failed */
        
        return 1;                                                                                        /* return error */
    }
    res = a_rx8025t_iic_write(handle, RX8025T_REG_MONTH, a_rx8025t_hex2bcd(t->month));                   /* write month and century */
    if (res != 0)                                                                                        /* check result */
    {
        handle->debug_print("rx8025t: write century and month failed.\n");                               /* write century and month failed */
        
        return 1;                                                                                        /* return error */
    }
    year = t->year - 2000;                                                                               /* year - 2000 */
    res = a_rx8025t_iic_write(handle, RX8025T_REG_YEAR, a_rx8025t_hex2bcd((uint8_t)year));               /* write year */
    if (res != 0)                                                                                        /* check result */
    {
        handle->debug_print("rx8025t: write year failed.\n");                                            /* write year failed */
        
        return 1;                                                                                        /* return error */
    }
    
    return 0;                                                                                            /* success return 0 */
}

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
uint8_t rx8025t_get_time(rx8025t_handle_t *handle, rx8025t_time_t *t)
{
    uint8_t res;
    uint8_t buf[7];
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    if (t == NULL)                                                                        /* check time */
    {
        handle->debug_print("rx8025t: time is null.\n");                                  /* time is null */
        
        return 2;                                                                         /* return error */
    }
    
    memset(buf, 0, sizeof(uint8_t) * 7);                                                  /* clear the buffer */
    res = a_rx8025t_iic_multiple_read(handle, RX8025T_REG_SEC, (uint8_t *)buf, 7);        /* multiple_read */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("rx8025t: multiple read failed.\n");                          /* multiple read failed */
        
        return 1;                                                                         /* return error */
    }
    t->year = a_rx8025t_bcd2hex(buf[6]) + 2000;                                           /* get year */
    t->month = a_rx8025t_bcd2hex(buf[5] & 0x1F);                                          /* get month */
    t->week = a_week_convert(buf[3] & 0x7F);                                              /* get week */
    if (t->week == 0)                                                                     /* check sunday */
    {
        t->week = 7;                                                                      /* set sunday */
    }
    t->date = a_rx8025t_bcd2hex(buf[4] & 0x3F);                                           /* get date */
    t->hour = a_rx8025t_bcd2hex(buf[2] & 0x3F);                                           /* get hour */
    t->minute = a_rx8025t_bcd2hex(buf[1] & 0x7F);                                         /* get minute */
    t->second = a_rx8025t_bcd2hex(buf[0] & 0x7F);                                         /* get second */
    
    return 0;                                                                             /* success return 0 */
}

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
uint8_t rx8025t_set_minute_alarm(rx8025t_handle_t *handle, uint8_t minute)
{
    uint8_t res;
    
    if (handle == NULL)                                                                         /* check handle */
    {
        return 2;                                                                               /* return error */
    }
    if (handle->inited != 1)                                                                    /* check handle initialization */
    {
        return 3;                                                                               /* return error */
    }
    if (minute > 59)                                                                            /* check minute */
    {
        handle->debug_print("rx8025t: minute can't be over than 59.\n");                        /* minute can't be over than 59 */
        
        return 4;                                                                               /* return error */
    }
    
    res = a_rx8025t_iic_write(handle, RX8025T_REG_MIN_ALARM, a_rx8025t_hex2bcd(minute));        /* write minute alarm */
    if (res != 0)                                                                               /* check result */
    {
        handle->debug_print("rx8025t: write minute alarm failed.\n");                           /* write minute alarm failed */
        
        return 1;                                                                               /* return error */
    }
    
    return 0;                                                                                   /* success return 0 */
}

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
uint8_t rx8025t_get_minute_alarm(rx8025t_handle_t *handle, uint8_t *minute)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                           /* check handle */
    {
        return 2;                                                                                 /* return error */
    }
    if (handle->inited != 1)                                                                      /* check handle initialization */
    {
        return 3;                                                                                 /* return error */
    }
    
    res = a_rx8025t_iic_multiple_read(handle, RX8025T_REG_MIN_ALARM, (uint8_t *)&prev, 1);        /* read minute alarm */
    if (res != 0)                                                                                 /* check result */
    {
        handle->debug_print("rx8025t: read minute alarm failed.\n");                              /* read minute alarm failed */
        
        return 1;                                                                                 /* return error */
    }
    *minute = a_rx8025t_bcd2hex(prev & 0x7F);                                                     /* set minute */
    
    return 0;                                                                                     /* success return 0 */
}

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
uint8_t rx8025t_set_hour_alarm(rx8025t_handle_t *handle, uint8_t hour)
{
    uint8_t res;
    
    if (handle == NULL)                                                                        /* check handle */
    {
        return 2;                                                                              /* return error */
    }
    if (handle->inited != 1)                                                                   /* check handle initialization */
    {
        return 3;                                                                              /* return error */
    }
    if (hour > 23)                                                                             /* check hour */
    {
        handle->debug_print("rx8025t: hour can't be over than 23.\n");                         /* hour can't be over than 23 */
        
        return 4;                                                                              /* return error */
    }
    
    res = a_rx8025t_iic_write(handle, RX8025T_REG_HOUR_ALARM, a_rx8025t_hex2bcd(hour));        /* write hour alarm */
    if (res != 0)                                                                              /* check result */
    {
        handle->debug_print("rx8025t: write hour alarm failed.\n");                            /* write hour alarm failed */
        
        return 1;                                                                              /* return error */
    }
    
    return 0;                                                                                  /* success return 0 */
}

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
uint8_t rx8025t_get_hour_alarm(rx8025t_handle_t *handle, uint8_t *hour)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                            /* check handle */
    {
        return 2;                                                                                  /* return error */
    }
    if (handle->inited != 1)                                                                       /* check handle initialization */
    {
        return 3;                                                                                  /* return error */
    }
    
    res = a_rx8025t_iic_multiple_read(handle, RX8025T_REG_HOUR_ALARM, (uint8_t *)&prev, 1);        /* read hour alarm */
    if (res != 0)                                                                                  /* check result */
    {
        handle->debug_print("rx8025t: read hour alarm failed.\n");                                 /* read hour alarm failed */
        
        return 1;                                                                                  /* return error */
    }
    *hour = a_rx8025t_bcd2hex(prev & 0x3F);                                                        /* set hour */
    
    return 0;                                                                                      /* success return 0 */
}

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
uint8_t rx8025t_set_date_alarm(rx8025t_handle_t *handle, uint8_t date)
{
    uint8_t res;
    
    if (handle == NULL)                                                                             /* check handle */
    {
        return 2;                                                                                   /* return error */
    }
    if (handle->inited != 1)                                                                        /* check handle initialization */
    {
        return 3;                                                                                   /* return error */
    }
    if ((date == 0) || (date > 31))                                                                 /* check data */
    {
        handle->debug_print("rx8025t: date can't be zero or over than 31.\n");                      /* date can't be zero or over than 31 */
        
        return 4;                                                                                   /* return error */
    }
    
    res = a_rx8025t_iic_write(handle, RX8025T_REG_WEEK_DAY_ALARM, a_rx8025t_hex2bcd(date));        /* week day alarm */
    if (res != 0)                                                                                  /* check result */
    {
        handle->debug_print("rx8025t: week day alarm failed.\n");                                  /* week day alarm failed */
        
        return 1;                                                                                  /* return error */
    }
    
    return 0;                                                                                      /* success return 0 */
}

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
uint8_t rx8025t_get_date_alarm(rx8025t_handle_t *handle, uint8_t *date)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                                /* check handle */
    {
        return 2;                                                                                      /* return error */
    }
    if (handle->inited != 1)                                                                           /* check handle initialization */
    {
        return 3;                                                                                      /* return error */
    }
    
    res = a_rx8025t_iic_multiple_read(handle, RX8025T_REG_WEEK_DAY_ALARM, (uint8_t *)&prev, 1);        /* read day alarm */
    if (res != 0)                                                                                      /* check result */
    {
        handle->debug_print("rx8025t: read day alarm failed.\n");                                      /* read day alarm failed */
        
        return 1;                                                                                      /* return error */
    }
    *date = a_rx8025t_bcd2hex(prev & 0x3F);                                                            /* set date */
    
    return 0;                                                                                          /* success return 0 */
}

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
uint8_t rx8025t_set_week_alarm(rx8025t_handle_t *handle, uint8_t week)
{
    uint8_t res;
    uint8_t reg;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    if ((week == 0) || (week > 7))                                                   /* check week */
    {
        handle->debug_print("rx8025t: week can't be zero or over than 7.\n");        /* week can't be zero or over than 7 */
        
        return 4;                                                                    /* return error */
    }
    
    if (week == 7)                                                                   /* sunday */
    {
        reg = 0;                                                                     /* sunday set bit 0 */
    }
    else
    {
        reg = week;                                                                  /* set week bit */
    }
    res = a_rx8025t_iic_write(handle, RX8025T_REG_WEEK_DAY_ALARM, 1 << reg);         /* week day alarm */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("rx8025t: week day alarm failed.\n");                    /* week day alarm failed */
        
        return 1;                                                                    /* return error */
    }
    
    return 0;                                                                        /* success return 0 */
}

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
uint8_t rx8025t_get_week_alarm(rx8025t_handle_t *handle, uint8_t *week)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                                /* check handle */
    {
        return 2;                                                                                      /* return error */
    }
    if (handle->inited != 1)                                                                           /* check handle initialization */
    {
        return 3;                                                                                      /* return error */
    }
    
    res = a_rx8025t_iic_multiple_read(handle, RX8025T_REG_WEEK_DAY_ALARM, (uint8_t *)&prev, 1);        /* read day alarm */
    if (res != 0)                                                                                      /* check result */
    {
        handle->debug_print("rx8025t: read day alarm failed.\n");                                      /* read day alarm failed */
        
        return 1;                                                                                      /* return error */
    }
    *week = a_week_convert(prev & 0x7F);                                                               /* set week */
    if ((*week) == 0)                                                                                  /* check sunday */
    {
        (*week) = 7;                                                                                   /* set sunday */
    }
    
    return 0;                                                                                          /* success return 0 */
}

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
uint8_t rx8025t_clear_flag(rx8025t_handle_t *handle)
{
    uint8_t res;
    
    if (handle == NULL)                                               /* check handle */
    {
        return 2;                                                     /* return error */
    }
    if (handle->inited != 1)                                          /* check handle initialization */
    {
        return 3;                                                     /* return error */
    }
    
    res = a_rx8025t_iic_write(handle, RX8025T_REG_FLAG, 0x00);        /* write flag */
    if (res != 0)                                                     /* check result */
    {
        handle->debug_print("rx8025t: write flag failed.\n");         /* write flag failed */
        
        return 1;                                                     /* return error */
    }
    
    return 0;                                                         /* success return 0 */
}

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
uint8_t rx8025t_set_minute_alarm_mask(rx8025t_handle_t *handle, rx8025t_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                           /* check handle */
    {
        return 2;                                                                                 /* return error */
    }
    if (handle->inited != 1)                                                                      /* check handle initialization */
    {
        return 3;                                                                                 /* return error */
    }
    
    res = a_rx8025t_iic_multiple_read(handle, RX8025T_REG_MIN_ALARM, (uint8_t *)&prev, 1);        /* read minute alarm */
    if (res != 0)                                                                                 /* check result */
    {
        handle->debug_print("rx8025t: read minute alarm failed.\n");                              /* read minute alarm failed */
        
        return 1;                                                                                 /* return error */
    }
    prev &= ~(1 << 7);                                                                            /* clear settings */
    prev |= enable << 7;                                                                          /* set bool */
    res = a_rx8025t_iic_write(handle, RX8025T_REG_MIN_ALARM, prev);                               /* write minute alarm */
    if (res != 0)                                                                                 /* check result */
    {
        handle->debug_print("rx8025t: write minute alarm failed.\n");                             /* write minute alarm failed */
        
        return 1;                                                                                 /* return error */
    }
    
    return 0;                                                                                     /* success return 0 */
}

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
uint8_t rx8025t_get_minute_alarm_mask(rx8025t_handle_t *handle, rx8025t_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                           /* check handle */
    {
        return 2;                                                                                 /* return error */
    }
    if (handle->inited != 1)                                                                      /* check handle initialization */
    {
        return 3;                                                                                 /* return error */
    }
    
    res = a_rx8025t_iic_multiple_read(handle, RX8025T_REG_MIN_ALARM, (uint8_t *)&prev, 1);        /* read minute alarm */
    if (res != 0)                                                                                 /* check result */
    {
        handle->debug_print("rx8025t: read minute alarm failed.\n");                              /* read minute alarm failed */
        
        return 1;                                                                                 /* return error */
    }
    *enable = (rx8025t_bool_t)((prev >> 7) & 0x01);                                               /* set bool */
    
    return 0;                                                                                     /* success return 0 */
}

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
uint8_t rx8025t_set_hour_alarm_mask(rx8025t_handle_t *handle, rx8025t_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                            /* check handle */
    {
        return 2;                                                                                  /* return error */
    }
    if (handle->inited != 1)                                                                       /* check handle initialization */
    {
        return 3;                                                                                  /* return error */
    }
    
    res = a_rx8025t_iic_multiple_read(handle, RX8025T_REG_HOUR_ALARM, (uint8_t *)&prev, 1);        /* read hour alarm */
    if (res != 0)                                                                                  /* check result */
    {
        handle->debug_print("rx8025t: read hour alarm failed.\n");                                 /* read hour alarm failed */
        
        return 1;                                                                                  /* return error */
    }
    prev &= ~(1 << 7);                                                                             /* clear settings */
    prev |= enable << 7;                                                                           /* set bool */
    res = a_rx8025t_iic_write(handle, RX8025T_REG_HOUR_ALARM, prev);                               /* write hour alarm */
    if (res != 0)                                                                                  /* check result */
    {
        handle->debug_print("rx8025t: write hour alarm failed.\n");                                /* write hour alarm failed */
        
        return 1;                                                                                  /* return error */
    }
    
    return 0;                                                                                      /* success return 0 */
}

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
uint8_t rx8025t_get_hour_alarm_mask(rx8025t_handle_t *handle, rx8025t_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                            /* check handle */
    {
        return 2;                                                                                  /* return error */
    }
    if (handle->inited != 1)                                                                       /* check handle initialization */
    {
        return 3;                                                                                  /* return error */
    }
    
    res = a_rx8025t_iic_multiple_read(handle, RX8025T_REG_HOUR_ALARM, (uint8_t *)&prev, 1);        /* read hour alarm */
    if (res != 0)                                                                                  /* check result */
    {
        handle->debug_print("rx8025t: read hour alarm failed.\n");                                 /* read hour alarm failed */
        
        return 1;                                                                                  /* return error */
    }
    *enable = (rx8025t_bool_t)((prev >> 7) & 0x01);                                                /* set bool */
    
    return 0;                                                                                      /* success return 0 */
}

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
uint8_t rx8025t_set_week_day_alarm_mask(rx8025t_handle_t *handle, rx8025t_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                                /* check handle */
    {
        return 2;                                                                                      /* return error */
    }
    if (handle->inited != 1)                                                                           /* check handle initialization */
    {
        return 3;                                                                                      /* return error */
    }
    
    res = a_rx8025t_iic_multiple_read(handle, RX8025T_REG_WEEK_DAY_ALARM, (uint8_t *)&prev, 1);        /* read week day alarm */
    if (res != 0)                                                                                      /* check result */
    {
        handle->debug_print("rx8025t: read week day alarm failed.\n");                                 /* read week day alarm failed */
        
        return 1;                                                                                      /* return error */
    }
    prev &= ~(1 << 7);                                                                                 /* clear settings */
    prev |= enable << 7;                                                                               /* set bool */
    res = a_rx8025t_iic_write(handle, RX8025T_REG_WEEK_DAY_ALARM, prev);                               /* write week day alarm */
    if (res != 0)                                                                                      /* check result */
    {
        handle->debug_print("rx8025t: write week day alarm failed.\n");                                /* write week day alarm failed */
        
        return 1;                                                                                      /* return error */
    }
    
    return 0;                                                                                          /* success return 0 */
}

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
uint8_t rx8025t_get_week_day_alarm_mask(rx8025t_handle_t *handle, rx8025t_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                                /* check handle */
    {
        return 2;                                                                                      /* return error */
    }
    if (handle->inited != 1)                                                                           /* check handle initialization */
    {
        return 3;                                                                                      /* return error */
    }
    
    res = a_rx8025t_iic_multiple_read(handle, RX8025T_REG_WEEK_DAY_ALARM, (uint8_t *)&prev, 1);        /* read week day alarm */
    if (res != 0)                                                                                      /* check result */
    {
        handle->debug_print("rx8025t: read week day alarm failed.\n");                                 /* read week day alarm failed */
        
        return 1;                                                                                      /* return error */
    }
    *enable = (rx8025t_bool_t)((prev >> 7) & 0x01);                                                    /* set bool */
    
    return 0;                                                                                          /* success return 0 */
}

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
uint8_t rx8025t_irq_handler(rx8025t_handle_t *handle)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    res = a_rx8025t_iic_multiple_read(handle, RX8025T_REG_FLAG, &prev, 1);               /* read flag */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("rx8025t: read flag failed.\n");                             /* read flag failed */
        
        return 1;                                                                        /* return error */
    }
    res = a_rx8025t_iic_write(handle, RX8025T_REG_FLAG, 0x00);                           /* write flag */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("rx8025t: write flag failed.\n");                            /* write flag failed */
        
        return 1;                                                                        /* return error */
    }
    if ((prev & (1 << 5)) != 0)                                                          /* check update flag */
    {
        if (handle->receive_callback != NULL)                                            /* check the callback */
        {
            handle->receive_callback(RX8025T_STATUS_UPDATE);                             /* run the callback */
        }
    }
    if ((prev & (1 << 4)) != 0)                                                          /* check timer flag */
    {
        if (handle->receive_callback != NULL)                                            /* check the callback */
        {
            handle->receive_callback(RX8025T_STATUS_TIMER);                              /* run the callback */
        }
    }
    if ((prev & (1 << 3)) != 0)                                                          /* check alarm flag */
    {
        if (handle->receive_callback != NULL)                                            /* check the callback */
        {
            handle->receive_callback(RX8025T_STATUS_ALARM);                              /* run the callback */
        }
    }
    if ((prev & (1 << 1)) != 0)                                                          /* check voltage low flag */
    {
        if (handle->receive_callback != NULL)                                            /* check the callback */
        {
            handle->receive_callback(RX8025T_STATUS_VOLTAGE_LOW);                        /* run the callback */
        }
    }
    if ((prev & (1 << 0)) != 0)                                                          /* check voltage detection flag */
    {
        if (handle->receive_callback != NULL)                                            /* check the callback */
        {
            handle->receive_callback(RX8025T_STATUS_VOLTAGE_DETECTION);                  /* run the callback */
        }
    }
    
    return 0;                                                                            /* success return 0 */
}

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
uint8_t rx8025t_init(rx8025t_handle_t *handle)
{
    if (handle == NULL)                                              /* check handle */
    {
        return 2;                                                    /* return error */
    }
    if (handle->debug_print == NULL)                                 /* check debug_print */
    {
        return 3;                                                    /* return error */
    }
    if (handle->iic_init == NULL)                                    /* check iic_init */
    {
        handle->debug_print("rx8025t: iic_init is null.\n");         /* iic_init is null */
       
        return 3;                                                    /* return error */
    }
    if (handle->iic_deinit == NULL)                                  /* check iic_deinit */
    {
        handle->debug_print("rx8025t: iic_deinit is null.\n");       /* iic_deinit is null */
       
        return 3;                                                    /* return error */
    }
    if (handle->iic_write == NULL)                                   /* check iic_write */
    {
        handle->debug_print("rx8025t: iic_write is null.\n");        /* iic_write is null */
       
        return 3;                                                    /* return error */
    }
    if (handle->iic_read == NULL)                                    /* check iic_read */
    {
        handle->debug_print("rx8025t: iic_read is null.\n");         /* iic_read is null */
       
        return 3;                                                    /* return error */
    }
    if (handle->delay_ms == NULL)                                    /* check delay_ms */
    {
        handle->debug_print("rx8025t: delay_ms is null.\n");         /* delay_ms is null */
       
        return 3;                                                    /* return error */
    }
    
    if (handle->iic_init() != 0)                                     /* iic init */
    {
        handle->debug_print("rx8025t: iic init failed.\n");          /* iic init failed */
       
        return 1;                                                    /* return error */
    }
    handle->inited = 1;                                              /* flag finish initialization */
    
    return 0;                                                        /* success return 0 */
}

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
uint8_t rx8025t_deinit(rx8025t_handle_t *handle)
{
    if (handle == NULL)                                             /* check handle */
    {
        return 2;                                                   /* return error */
    }
    if (handle->inited != 1)                                        /* check handle initialization */
    {
        return 3;                                                   /* return error */
    }
    
    if (handle->iic_deinit() != 0)                                  /* iic deinit */
    {
        handle->debug_print("rx8025t: iic deinit failed.\n");       /* iic deinit failed */
       
        return 1;                                                   /* return error */
    }
    handle->inited = 0;                                             /* flag close */
    
    return 0;                                                       /* success return 0 */
}

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
uint8_t rx8025t_set_reg(rx8025t_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle == NULL)                                                /* check handle */
    {
        return 2;                                                      /* return error */
    }
    if (handle->inited != 1)                                           /* check handle initialization */
    {
        return 3;                                                      /* return error */
    }
    
    if (handle->iic_write(RX8025T_ADDRESS, reg, buf, len) != 0)        /* write data */
    {
        return 1;                                                      /* return error */
    }
    
    return 0;                                                          /* success return 0 */
}

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
uint8_t rx8025t_get_reg(rx8025t_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle == NULL)                                               /* check handle */
    {
        return 2;                                                     /* return error */
    }
    if (handle->inited != 1)                                          /* check handle initialization */
    {
        return 3;                                                     /* return error */
    }
    
    if (handle->iic_read(RX8025T_ADDRESS, reg, buf, len) != 0)        /* read data */
    {
        return 1;                                                     /* return error */
    }
    
    return 0;                                                         /* success return 0 */
}

/**
 * @brief      get chip's information
 * @param[out] *info pointer to a rx8025t info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t rx8025t_info(rx8025t_info_t *info)
{
    if (info == NULL)                                               /* check handle */
    {
        return 2;                                                   /* return error */
    }
    
    memset(info, 0, sizeof(rx8025t_info_t));                        /* initialize rx8025t info structure */
    strncpy(info->chip_name, CHIP_NAME, 32);                        /* copy chip name */
    strncpy(info->manufacturer_name, MANUFACTURER_NAME, 32);        /* copy manufacturer name */
    strncpy(info->interface, "IIC", 8);                             /* copy interface name */
    info->supply_voltage_min_v = SUPPLY_VOLTAGE_MIN;                /* set minimal supply voltage */
    info->supply_voltage_max_v = SUPPLY_VOLTAGE_MAX;                /* set maximum supply voltage */
    info->max_current_ma = MAX_CURRENT;                             /* set maximum current */
    info->temperature_max = TEMPERATURE_MAX;                        /* set minimal temperature */
    info->temperature_min = TEMPERATURE_MIN;                        /* set maximum temperature */
    info->driver_version = DRIVER_VERSION;                          /* set driver version */
    
    return 0;                                                       /* success return 0 */
}
