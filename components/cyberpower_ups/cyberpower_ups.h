#pragma once

#include "esphome.h"
#include "usb/usb_host.h"
#include "hid_host.h" 

#define UPS_MAX_WATTS 810 // Default config

namespace esphome {
namespace cyberpower_ups {

class CyberPowerUPS : public PollingComponent {
 public:
  sensor::Sensor *watt_sensor = nullptr;
  sensor::Sensor *va_sensor = nullptr;
  sensor::Sensor *load_sensor = nullptr;
  sensor::Sensor *battery_sensor = nullptr;
  binary_sensor::BinarySensor *online_sensor = nullptr;

  void set_watt_sensor(sensor::Sensor *s) { watt_sensor = s; }
  void set_va_sensor(sensor::Sensor *s) { va_sensor = s; }
  void set_load_sensor(sensor::Sensor *s) { load_sensor = s; }
  void set_battery_sensor(sensor::Sensor *s) { battery_sensor = s; }
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

  // --- USB BACKGROUND TASK ---
  static void usb_lib_task(void *arg) {
    while (1) {
      uint32_t event_flags;
      usb_host_lib_handle_events(portMAX_DELAY, &event_flags);
      if (event_flags & USB_HOST_LIB_EVENT_FLAGS_NO_CLIENTS) {
        usb_host_device_free_all();
      }
    }
  }

  // --- DATA PARSER ---
  static void event_callback_trampoline(hid_host_device_handle_t handle, 
                                        const hid_host_interface_event_t event, 
                                        void *arg) {
      CyberPowerUPS *self = (CyberPowerUPS *)arg;
      self->process_report(handle, event);
  }

  void process_report(hid_host_device_handle_t handle, const hid_host_interface_event_t event) {
    uint8_t data[64];
    size_t length = 0;

    if (event == HID_HOST_INTERFACE_EVENT_INPUT_REPORT) {
      if (hid_host_device_get_raw_input_report_data(handle, data, 64, &length) == ESP_OK) {
         for (size_t i = 0; i < length - 1; i += 2) {
             uint8_t id = data[i];
             uint8_t val = data[i+1];

             switch (id) {
                case 0x08: state.battery = val; state.updated = true; break;
                case 0x0B: state.is_online = !(val & 0x04); state.updated = true; break;
                case 0x1D: state.va = val; state.updated = true; break;
                case 0x19: 
                    state.watts = val; 
                    state.load = (val * 100) / UPS_MAX_WATTS;
                    state.updated = true;
                    break;
             }
         }
      }
    }
  }

  // --- CONNECTION HANDLING ---
  static void device_callback_trampoline(hid_host_device_handle_t handle, 
                                         const hid_host_driver_event_t event, 
                                         void *arg) {
     CyberPowerUPS *self = (CyberPowerUPS *)arg;
     if (event == HID_HOST_DRIVER_EVENT_CONNECTED) {
        ESP_LOGI("UPS", "Connected! Starting Driver...");
        const hid_host_device_config_t dev_config = { 
            .callback = event_callback_trampoline, 
            .callback_arg = self 
        };
        if (hid_host_device_open(handle, &dev_config) == ESP_OK) {
            hid_host_device_start(handle);
        }
     }
  }

  void setup() override {
    const usb_host_config_t host_config = { .skip_phy_setup = false, .intr_flags = ESP_INTR_FLAG_LEVEL1 };
    usb_host_install(&host_config);
    xTaskCreate(usb_lib_task, "usb_events", 4096, NULL, 2, NULL);

    const hid_host_driver_config_t hid_config = {
        .create_background_task = true,
        .task_priority = 5,
        .stack_size = 4096,
        .core_id = 0,
        .callback = device_callback_trampoline,
        .callback_arg = this 
    };
    hid_host_install(&hid_config);
  }

  float calculated_runtime = 0;

  void update() override {
      if (state.updated) {
          // --- Runtime Calculation ---
          // Assumption: 24V system, 9Ah batteries, 0.8 efficiency factor
          // Change 216 (24V * 9Ah) if your battery specs differ
          if (state.watts > 5) { 
              state.calculated_runtime = (216.0f * 0.8f / (float)state.watts) * 60.0f;
          } else {
              state.calculated_runtime = 480; // Default max (8 hours) if no load
          }
  
          // --- Publish to Sensors ---
          if (watt_sensor) watt_sensor->publish_state(state.watts);
          if (va_sensor) va_sensor->publish_state(state.va);
          if (load_sensor) load_sensor->publish_state(state.load);
          if (battery_sensor) battery_sensor->publish_state(state.battery);
          
          // --- Binary Sensor Logic ---
          if (online_sensor) online_sensor->publish_state(state.is_online);
          
          state.updated = false;
      }
}
};

} // namespace cyberpower_ups
} // namespace esphome
