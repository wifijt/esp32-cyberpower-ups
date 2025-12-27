#pragma once
#include "esphome.h"
#include "usb/usb_host.h"
#include "hid_host.h"

namespace esphome {
namespace cyberpower_ups {

class CyberPowerUPS : public PollingComponent {
 public:
  sensor::Sensor *watt_sensor = nullptr;
  sensor::Sensor *va_sensor = nullptr;
  sensor::Sensor *load_sensor = nullptr;
  sensor::Sensor *battery_sensor = nullptr;
  sensor::Sensor *runtime_sensor = nullptr;
  binary_sensor::BinarySensor *online_sensor = nullptr;

  float max_watts_ = 810.0f;
  void set_max_watts(float max_watts) { this->max_watts_ = max_watts; }

  void set_watt_sensor(sensor::Sensor *s) { watt_sensor = s; }
  void set_va_sensor(sensor::Sensor *s) { va_sensor = s; }
  void set_load_sensor(sensor::Sensor *s) { load_sensor = s; }
  void set_battery_sensor(sensor::Sensor *s) { battery_sensor = s; }
  void set_runtime_sensor(sensor::Sensor *s) { runtime_sensor = s; }
  void set_online_sensor(binary_sensor::BinarySensor *s) { online_sensor = s; }

  struct {
     int battery = 0;
     int watts = 0;
     int va = 0;
     int load = 0;
     bool is_online = true;
     bool updated = false;
  } state;

  CyberPowerUPS() : PollingComponent(1000) {}

  void setup() override {
    xTaskCreatePinnedToCore(usb_lib_task, "usb_events", 8192, this, 2, NULL, 0);
  }

  static void hid_host_interface_callback(hid_host_device_handle_t hid_device_handle,
                                         const hid_host_interface_event_t event,
                                         void *arg) {
    CyberPowerUPS *ups = (CyberPowerUPS *)arg;
    uint8_t data[64];
    size_t data_len;
    if (event == HID_HOST_INTERFACE_EVENT_INPUT_REPORT) {
        if (hid_host_device_get_raw_input_report_data(hid_device_handle, data, 64, &data_len) == ESP_OK) {
            ups->state.battery = data[1]; 
            ups->state.watts = data[3];   
            ups->state.is_online = (data[2] & 0x01); 
            ups->state.updated = true;
        }
    }
  }

  // THIS IS THE HANDSHAKE YOUR REPO CURRENTLY LACKS
  static void hid_host_device_event_callback(hid_host_device_handle_t hid_device_handle,
                                            const hid_host_driver_event_t event,
                                            void *arg) {
    if (event == HID_HOST_DRIVER_EVENT_CONNECTED) {
        CyberPowerUPS *ups = (CyberPowerUPS *)arg;
        const hid_host_device_config_t dev_config = {
            .callback = hid_host_interface_callback,
            .callback_arg = ups
        };
        hid_host_device_open(hid_device_handle, &dev_config);
        hid_host_device_start(hid_device_handle);
    }
  }

  static void usb_lib_task(void *arg) {
    CyberPowerUPS *ups = (CyberPowerUPS *)arg;
    const hid_host_driver_config_t driver_config = {
        .create_background_task = true,
        .task_priority = 5,
        .stack_size = 4096,
        .core_id = 0,
        .callback = hid_host_device_event_callback, // The fix
        .callback_arg = ups
    };
    hid_host_install(&driver_config);
    while (true) {
        hid_host_handle_events(50);
        vTaskDelay(pdMS_TO_TICKS(50));
    }
  }

  void update() override {
    if (state.updated) {
        state.load = (int)((float)state.watts / max_watts_ * 100.0f);
        if (watt_sensor) watt_sensor->publish_state(state.watts);
        if (va_sensor) va_sensor->publish_state(state.va);
        if (load_sensor) load_sensor->publish_state(state.load);
        if (battery_sensor) battery_sensor->publish_state(state.battery);
        if (online_sensor) online_sensor->publish_state(state.is_online);
        if (runtime_sensor) {
            if (state.watts > 5) {
                float minutes = (216.0f * 0.8f / (float)state.watts) * 60.0f;
                runtime_sensor->publish_state(minutes > 480.0f ? 480.0f : minutes);
            } else {
                runtime_sensor->publish_state(480.0f); 
            }
        }
        state.updated = false;
    }
  }
};
} // namespace cyberpower_ups
} // namespace esphome
