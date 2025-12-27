/*
 * SPDX-FileCopyrightText: 2022-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

// Forward declaration
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

// Callback Typedefs (matching the C implementation)
typedef void (*hid_host_driver_event_callback_t)(hid_host_device_handle_t hid_device_handle,
                                                 const hid_host_driver_event_t event,
                                                 void *arg);
// Alias used in C file
typedef hid_host_driver_event_callback_t hid_host_driver_event_cb_t;

typedef void (*hid_host_interface_event_callback_t)(hid_host_device_handle_t hid_device_handle,
                                                    const hid_host_interface_event_t event,
                                                    void *arg);
// Alias used in C file
typedef hid_host_interface_event_callback_t hid_host_interface_event_cb_t;

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

// --- MISSING STRUCTURES FIX ---

/**
 * @brief HID Device Parameters
 */
typedef struct {
    uint8_t addr;
    uint8_t iface_num;
    uint8_t sub_class;
    uint8_t proto;
} hid_host_dev_params_t;

/**
 * @brief HID Device Info
 */
typedef struct {
    uint16_t vid;
    uint16_t pid;
    uint8_t sub_class;
    uint8_t proto;
} hid_host_dev_info_t;

/**
 * @brief HID Protocol Mode
 */
typedef enum {
    HID_REPORT_PROTOCOL_BOOT = 0,
    HID_REPORT_PROTOCOL_REPORT = 1
} hid_report_protocol_t;

// Standard HID Constants
#define HID_CLASS_DESCRIPTOR_TYPE_HID       0x21
#define HID_CLASS_DESCRIPTOR_TYPE_REPORT    0x22
#define HID_CLASS_DESCRIPTOR_TYPE_PHYSICAL  0x23

#define HID_REPORT_TYPE_INPUT       0x01
#define HID_REPORT_TYPE_OUTPUT      0x02
#define HID_REPORT_TYPE_FEATURE     0x03

#define HID_STR_DESC_MAX_LENGTH     128

// HID Descriptor Structure
typedef struct {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint16_t bcdHID;
    uint8_t bCountryCode;
    uint8_t bNumDescriptors;
    uint8_t bReportDescriptorType;
    uint16_t wReportDescriptorLength;
} __attribute__((packed)) hid_descriptor_t;

#ifdef __cplusplus
}
#endif
