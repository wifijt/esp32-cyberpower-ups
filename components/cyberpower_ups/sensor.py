import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import UNIT_WATT, UNIT_VOLT_AMPS, UNIT_PERCENT, DEVICE_CLASS_BATTERY, DEVICE_CLASS_POWER
from . import CyberPowerUPS, CONF_ID

CONF_WATTS = "watts"
CONF_VA = "va"
CONF_LOAD = "load"
CONF_BATTERY = "battery"

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.use_id(CyberPowerUPS),
    cv.Optional(CONF_WATTS): sensor.sensor_schema(unit_of_measurement=UNIT_WATT, accuracy_decimals=0, device_class=DEVICE_CLASS_POWER),
    cv.Optional(CONF_VA): sensor.sensor_schema(unit_of_measurement=UNIT_VOLT_AMPS, accuracy_decimals=0),
    cv.Optional(CONF_LOAD): sensor.sensor_schema(unit_of_measurement=UNIT_PERCENT, accuracy_decimals=0),
    cv.Optional(CONF_BATTERY): sensor.sensor_schema(unit_of_measurement=UNIT_PERCENT, accuracy_decimals=0, device_class=DEVICE_CLASS_BATTERY),
})

async def to_code(config):
    hub = await cg.get_variable(config[CONF_ID])
    if CONF_WATTS in config:
        sens = await sensor.new_sensor(config[CONF_WATTS])
        cg.add(hub.set_watt_sensor(sens))
    if CONF_VA in config:
        sens = await sensor.new_sensor(config[CONF_VA])
        cg.add(hub.set_va_sensor(sens))
    if CONF_LOAD in config:
        sens = await sensor.new_sensor(config[CONF_LOAD])
        cg.add(hub.set_load_sensor(sens))
    if CONF_BATTERY in config:
        sens = await sensor.new_sensor(config[CONF_BATTERY])
        cg.add(hub.set_battery_sensor(sens))
