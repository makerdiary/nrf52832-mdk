/**
 * Copyright (c) 2016 - 2017, Nordic Semiconductor ASA
 * 
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 * 
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 * 
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 * 
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 * 
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */
/** @file
 *
 * @defgroup ble_app_eddystone main.c
 * @{
 * @ingroup ble_app_eddystone
 * @brief Eddystone Beacon GATT Configuration Service + EID/eTLM sample application main file.
 *
 * This file contains the source code for an Eddystone
 * Beacon GATT Configuration Service + EID/eTLM sample application.
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "bsp.h"
#include "ble_conn_params.h"
#include "ble_advertising.h"
#include "softdevice_handler.h"
#include "app_timer.h"
#include "es_app_config.h"
#include "app_scheduler.h"
#include "nrf_ble_es.h"
#include "fstorage.h"
#include "nrf_ble_gatt.h"


#define DEAD_BEEF                   0xDEADBEEF          //!< Value used as error code on stack dump, can be used to identify stack location on stack unwind.
#define NON_CONNECTABLE_ADV_LED_PIN BSP_BOARD_LED_0     //!< Toggles when non-connectable advertisement is sent.
#define CONNECTED_LED_PIN           BSP_BOARD_LED_1     //!< Is on when device has connected.
#define CONNECTABLE_ADV_LED_PIN     BSP_BOARD_LED_2     //!< Is on when device is advertising connectable advertisements.


static nrf_ble_gatt_t m_gatt;                           //!< GATT module instance.


/**@brief Callback function for asserts in the SoftDevice.
 *
 * @details This function will be called in case of an assert in the SoftDevice.
 *
 * @warning This handler is an example only and does not fit a final product. You need to analyze
 *          how your product is supposed to react in case of Assert.
 * @warning On assert from the SoftDevice, the system can only recover on reset.
 *
 * @param[in]   line_num   Line number of the failing ASSERT call.
 * @param[in]   file_name  File name of the failing ASSERT call.
 */
void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name)
{
    app_error_handler(DEAD_BEEF, line_num, p_file_name);
}


/**@brief Function for the application's SoftDevice event handler.
 *
 * @param[in] p_ble_evt SoftDevice event.
 */
static void on_ble_evt(ble_evt_t * p_ble_evt)
{
    ret_code_t err_code;

    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_SEC_PARAMS_REQUEST:
            // Pairing not supported
            err_code = sd_ble_gap_sec_params_reply(p_ble_evt->evt.common_evt.conn_handle,
                                                   BLE_GAP_SEC_STATUS_PAIRING_NOT_SUPP,
                                                   NULL,
                                                   NULL);
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_GATTS_EVT_SYS_ATTR_MISSING:
            // No system attributes have been stored.
            err_code = sd_ble_gatts_sys_attr_set(p_ble_evt->evt.common_evt.conn_handle, NULL, 0, 0);
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_GAP_EVT_CONNECTED:
            bsp_board_led_on(CONNECTED_LED_PIN);
            bsp_board_led_off(CONNECTABLE_ADV_LED_PIN);
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            bsp_board_led_off(CONNECTED_LED_PIN);
            break;

        default:
            // No implementation needed.
            break;
    }
}


/**@brief Function for dispatching a SoftDevice event to all modules with a SoftDevice
 *        event handler.
 *
 * @details This function is called from the SoftDevice event interrupt handler after a
 *          SoftDevice event has been received.
 *
 * @param[in] p_ble_evt  SoftDevice event.
 */
static void ble_evt_dispatch(ble_evt_t * p_ble_evt)
{
    ble_conn_params_on_ble_evt(p_ble_evt);
    on_ble_evt(p_ble_evt);
    nrf_ble_es_on_ble_evt(p_ble_evt);
    ble_advertising_on_ble_evt(p_ble_evt);
    nrf_ble_gatt_on_ble_evt(&m_gatt, p_ble_evt);
}


/**@brief Function for handling system events from the SoftDevice.
 *
 * @param[in] evt SoftDevice system event.
 */
static void sys_evt_dispatch(uint32_t evt)
{
    fs_sys_event_handler(evt);
}


/**@brief Function for the GAP initialization.
*
* @details This function will set up all the necessary GAP (Generic Access Profile) parameters of
*          the device. It also sets the permissions and appearance.
*/
static void gap_params_init(void)
{
   ret_code_t              err_code;
   ble_gap_conn_params_t   gap_conn_params;
   ble_gap_conn_sec_mode_t sec_mode;
   uint8_t                 device_name[] = APP_DEVICE_NAME;

   BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);

   err_code = sd_ble_gap_device_name_set(&sec_mode,
                                         device_name,
                                         strlen((const char *)device_name));
   APP_ERROR_CHECK(err_code);

   memset(&gap_conn_params, 0, sizeof(gap_conn_params));

   gap_conn_params.min_conn_interval = MIN_CONN_INTERVAL;
   gap_conn_params.max_conn_interval = MAX_CONN_INTERVAL;
   gap_conn_params.slave_latency     = SLAVE_LATENCY;
   gap_conn_params.conn_sup_timeout  = CONN_SUP_TIMEOUT;

   err_code = sd_ble_gap_ppcp_set(&gap_conn_params);

   APP_ERROR_CHECK(err_code);
}


/**@brief Function for initializing the GATT module.
 */
static void gatt_init(void)
{
    ret_code_t err_code = nrf_ble_gatt_init(&m_gatt, NULL);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for initializing the BLE stack.
 *
 * @details Initializes the SoftDevice and the BLE event interrupt.
 */
static void ble_stack_init(void)
{
    ret_code_t         err_code;
    nrf_clock_lf_cfg_t lf_clock_config;

    lf_clock_config.source        = NRF_CLOCK_LF_SRC_XTAL;
    lf_clock_config.rc_ctiv       = 0;
    lf_clock_config.rc_temp_ctiv  = 0;
    lf_clock_config.xtal_accuracy = NRF_CLOCK_LF_XTAL_ACCURACY_20_PPM;

    // Initialize the SoftDevice handler module.
    SOFTDEVICE_HANDLER_INIT(&lf_clock_config, NULL);

    // Fetch the start address of the application RAM.
    uint32_t ram_start = 0;
    err_code = softdevice_app_ram_start_get(&ram_start);
    APP_ERROR_CHECK(err_code);

    // Overwrite some of the default configurations for the BLE stack.
    ble_cfg_t ble_cfg;

    // Configure the maximum number of connections.
    memset(&ble_cfg, 0, sizeof(ble_cfg));
    ble_cfg.gap_cfg.role_count_cfg.periph_role_count  = BLE_GAP_ROLE_COUNT_PERIPH_DEFAULT;
    ble_cfg.gap_cfg.role_count_cfg.central_role_count = 0;
    ble_cfg.gap_cfg.role_count_cfg.central_sec_count  = 0;
    err_code = sd_ble_cfg_set(BLE_GAP_CFG_ROLE_COUNT, &ble_cfg, ram_start);
    APP_ERROR_CHECK(err_code);

    // Enable BLE stack.
    err_code = softdevice_enable(&ram_start);
    APP_ERROR_CHECK(err_code);

    // Subscribe for BLE events.
    err_code = softdevice_ble_evt_handler_set(ble_evt_dispatch);
    APP_ERROR_CHECK(err_code);

    // Subscribe for system events.
    err_code = softdevice_sys_evt_handler_set(sys_evt_dispatch);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for initializing the Connection Parameters module.
 */
static void conn_params_init(void)
{
    ret_code_t             err_code;
    ble_conn_params_init_t cp_init;

    memset(&cp_init, 0, sizeof(cp_init));

    cp_init.p_conn_params                  = NULL;
    cp_init.first_conn_params_update_delay = FIRST_CONN_PARAMS_UPDATE_DELAY;
    cp_init.next_conn_params_update_delay  = NEXT_CONN_PARAMS_UPDATE_DELAY;
    cp_init.max_conn_params_update_count   = MAX_CONN_PARAMS_UPDATE_COUNT;
    cp_init.start_on_notify_cccd_handle    = BLE_GATT_HANDLE_INVALID;
    cp_init.disconnect_on_fail             = false;

    err_code = ble_conn_params_init(&cp_init);
    APP_ERROR_CHECK(err_code);

}


/**@brief Function for doing power management.
 */
static void power_manage(void)
{
    ret_code_t err_code = sd_app_evt_wait();
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for handling Eddystone events.
 *
 * @param[in] evt Eddystone event to handle.
 */
static void on_es_evt(nrf_ble_es_evt_t evt)
{
    switch(evt)
    {
        case NRF_BLE_ES_EVT_ADVERTISEMENT_SENT:
            bsp_board_led_invert(NON_CONNECTABLE_ADV_LED_PIN);
            break;

        case NRF_BLE_ES_EVT_CONNECTABLE_ADV_STARTED:
            bsp_board_led_on(CONNECTABLE_ADV_LED_PIN);
            break;

        default:
            break;
    }
}


/**@brief Function for handling button events from app_button IRQ
 *
 * @param[in] pin_no        Pin of the button for which an event has occured
 * @param[in] button_action Press or Release
 */
static void button_evt_handler(uint8_t pin_no, uint8_t button_action)
{
    if (button_action == APP_BUTTON_PUSH && pin_no == BUTTON_1)
    {
        nrf_ble_es_on_start_connectable_advertising();
    }
}


/**
 * @brief Function for initializing the registation button
 *
 * @retval Values returned by @ref app_button_init
 * @retval Values returned by @ref app_button_enable
 */
static void button_init(void)
{
    ret_code_t              err_code;
    const uint8_t           buttons_cnt  = 1;
    static app_button_cfg_t buttons_cfgs =
    {
        .pin_no         = BUTTON_REGISTRATION,
        .active_state   = APP_BUTTON_ACTIVE_LOW,
        .pull_cfg       = NRF_GPIO_PIN_PULLUP,
        .button_handler = button_evt_handler
    };

    err_code = app_button_init(&buttons_cfgs, buttons_cnt, APP_TIMER_TICKS(100));
    APP_ERROR_CHECK(err_code);

    err_code = app_button_enable();
    APP_ERROR_CHECK(err_code);
}


static void timers_init(void)
{
    ret_code_t err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);
}


static void leds_init(void)
{
    ret_code_t err_code = bsp_init(BSP_INIT_LED, NULL);
    APP_ERROR_CHECK(err_code);
}


static void scheduler_init(void)
{
    APP_SCHED_INIT(SCHED_MAX_EVENT_DATA_SIZE, SCHED_QUEUE_SIZE);
}


/**
 * @brief Function for application main entry.
 */
int main(void)
{
    timers_init();
    leds_init();
    button_init();
    scheduler_init();
    ble_stack_init();
    gap_params_init();
    gatt_init();
    conn_params_init();
    nrf_ble_es_init(on_es_evt);

    // Enter main loop.
    for (;;)
    {
        app_sched_execute();
        power_manage();
    }
}


/**
 * @}
 */
