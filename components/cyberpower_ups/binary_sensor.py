import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
from esphome.const import DEVICE_CLASS_POWER
from . import CyberPowerUPS, CONF_ID

CONF_ONLINE = "online_status"

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.use_id(CyberPowerUPS),
    cv.Optional(CONF_ONLINE): binary_sensor.binary_sensor_schema(device_class=DEVICE_CLASS_POWER),
})

async def to_code(config):
    hub = await cg.get_variable(config[CONF_ID])
    if CONF_ONLINE in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_ONLINE])
        cg.add(hub.set_online_sensor(sens))
