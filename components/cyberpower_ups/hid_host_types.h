/*
 * SPDX-FileCopyrightText: 2022-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct hid_host_device *hid_host_device_handle_t;

/**
 * @brief HID Host driver event
 */
typedef enum {
    HID_HOST_DRIVER_EVENT_CONNECTED,
    HID_HOST_DRIVER_EVENT_DISCONNECTED,
} hid_host_driver_event_t;

/**
 * @brief HID Host interface event
 */
typedef enum {
    HID_HOST_INTERFACE_EVENT_INPUT_REPORT,
    HID_HOST_INTERFACE_EVENT_DISCONNECTED,
    HID_HOST_INTERFACE_EVENT_TRANSFER_ERROR
} hid_host_interface_event_t;

/**
 * @brief HID Host driver event callback
 */
typedef void (*hid_host_driver_event_callback_t)(hid_host_device_handle_t hid_device_handle,
                                                 const hid_host_driver_event_t event,
                                                 void *arg);

/**
 * @brief HID Host interface event callback
 */
typedef void (*hid_host_interface_event_callback_t)(hid_host_device_handle_t hid_device_handle,
                                                    const hid_host_interface_event_t event,
                                                    void *arg);

/**
 * @brief HID Host driver configuration
 */
typedef struct {
    bool create_background_task;            /**< Create background task */
    uint32_t task_priority;                 /**< Background task priority */
    uint32_t stack_size;                    /**< Background task stack size */
    int core_id;                            /**< Background task core ID */
    hid_host_driver_event_callback_t callback; /**< Driver event callback */
    void *callback_arg;                     /**< Driver event callback argument */
} hid_host_driver_config_t;

/**
 * @brief HID Host device configuration
 */
typedef struct {
    hid_host_interface_event_callback_t callback; /**< Interface event callback */
    void *callback_arg;                           /**< Interface event callback argument */
} hid_host_device_config_t;

// Standard HID Report Types
#define HID_REPORT_TYPE_INPUT       0x01
#define HID_REPORT_TYPE_OUTPUT      0x02
#define HID_REPORT_TYPE_FEATURE     0x03

#ifdef __cplusplus
}
#endif