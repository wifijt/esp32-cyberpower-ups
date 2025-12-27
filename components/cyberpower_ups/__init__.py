import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID

DEPENDENCIES = ['esp32']
AUTO_LOAD = ['sensor', 'binary_sensor']

# Create the C++ namespace
cyberpower_ns = cg.esphome_ns.namespace('cyberpower_ups')
CyberPowerUPS = cyberpower_ns.class_('CyberPowerUPS', cg.PollingComponent)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(CyberPowerUPS),
}).extend(cv.polling_component_schema('10s'))

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
