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
  sensor::Sensor *runtime_sensor = nullptr; // <--- This must exist
  binary_sensor::BinarySensor *online_sensor = nullptr;

  void set_watt_sensor(sensor::Sensor *s) { watt_sensor = s; }
  void set_va_sensor(sensor::Sensor *s) { va_sensor = s; }
  void set_load_sensor(sensor::Sensor *s) { load_sensor = s; }
  void set_battery_sensor(sensor::Sensor *s) { battery_sensor = s; }
  // FIX: This is the function the Python script (main.cpp) was looking for
  void set_runtime_sensor(sensor::Sensor *s) { runtime_sensor = s; } 
  void set_online_sensor(binary_sensor::BinarySensor *s) { online_sensor = s; }

  struct {
     int battery = 0;
     int watts = 0;
     int va = 0;
     int load = 0;
     bool is_online = true;
     bool updated = false;
     // FIX: We define this here so the compiler knows what 'state.calculated_runtime' is
     float calculated_runtime = 0; 
  } state;

  CyberPowerUPS() : PollingComponent(1000) {}

  // ... [Keep your existing usb_lib_task and callbacks here] ...

  void update() override {
    if (state.updated) {
        if (watt_sensor) watt_sensor->publish_state(state.watts);
        if (va_sensor) va_sensor->publish_state(state.va);
        if (load_sensor) load_sensor->publish_state(state.load);
        if (battery_sensor) battery_sensor->publish_state(state.battery);
        if (online_sensor) online_sensor->publish_state(state.is_online);

        // Calculate and publish runtime
        if (runtime_sensor) {
            if (state.watts > 5) {
                state.calculated_runtime = (216.0f * 0.8f / (float)state.watts) * 60.0f;
            } else {
                state.calculated_runtime = 480.0f; 
            }
            runtime_sensor->publish_state(state.calculated_runtime > 480.0f ? 480.0f : state.calculated_runtime);
        }
        
        state.updated = false;
    }
  }
};

} // namespace cyberpower_ups
} // namespace esphome
