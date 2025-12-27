/*
 * SPDX-FileCopyrightText: 2022-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"
#include "hid_host_types.h"

#ifdef __cplusplus
extern "C" {
#endif

// --- Main Driver Functions ---
esp_err_t hid_host_install(const hid_host_driver_config_t *config);
esp_err_t hid_host_uninstall(void);

esp_err_t hid_host_device_open(hid_host_device_handle_t hid_device_handle,
                               const hid_host_device_config_t *config);
esp_err_t hid_host_device_close(hid_host_device_handle_t hid_device_handle);

esp_err_t hid_host_device_start(hid_host_device_handle_t hid_device_handle);
esp_err_t hid_host_device_stop(hid_host_device_handle_t hid_device_handle);

// --- Data & Info Functions ---
esp_err_t hid_host_device_get_raw_input_report_data(hid_host_device_handle_t hid_device_handle,
                                                    uint8_t *data,
                                                    size_t data_len,
                                                    size_t *actual_data_len);

esp_err_t hid_host_device_get_report(hid_host_device_handle_t hid_device_handle,
                                     uint8_t type,
                                     uint8_t report_id,
                                     uint8_t *data,
                                     size_t data_len);

esp_err_t hid_host_device_get_params(hid_host_device_handle_t hid_dev_handle,
                                     hid_host_dev_params_t *dev_params);

esp_err_t hid_host_get_device_info(hid_host_device_handle_t hid_dev_handle,
                                   hid_host_dev_info_t *hid_dev_info);

uint8_t *hid_host_get_report_descriptor(hid_host_device_handle_t hid_dev_handle,
                                        size_t *report_desc_len);

// --- Class Requests (Resolves Implicit Declaration Errors) ---
esp_err_t hid_class_request_get_report(hid_host_device_handle_t hid_dev_handle,
                                       uint8_t type,
                                       uint8_t report_id,
                                       uint8_t *data,
                                       size_t *data_len);

esp_err_t hid_class_request_get_idle(hid_host_device_handle_t hid_dev_handle,
                                     uint8_t report_id,
                                     uint8_t *idle_rate);

esp_err_t hid_class_request_get_protocol(hid_host_device_handle_t hid_dev_handle,
                                         hid_report_protocol_t *protocol);

esp_err_t hid_class_request_set_report(hid_host_device_handle_t hid_dev_handle,
                                       uint8_t type,
                                       uint8_t report_id,
                                       uint8_t *data,
                                       size_t data_len);

esp_err_t hid_class_request_set_idle(hid_host_device_handle_t hid_dev_handle,
                                     uint8_t report_id,
                                     uint8_t idle_rate);

esp_err_t hid_class_request_set_protocol(hid_host_device_handle_t hid_dev_handle,
                                         hid_report_protocol_t protocol);

#ifdef __cplusplus
}
#endif
