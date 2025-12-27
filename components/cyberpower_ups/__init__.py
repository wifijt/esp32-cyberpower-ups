import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID

# Define the key
CONF_MAX_WATTS = "max_watts"

cyberpower_ns = cg.esphome_ns.namespace('cyberpower_ups')
CyberPowerUPS = cyberpower_ns.class_('CyberPowerUPS', cg.PollingComponent)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(CyberPowerUPS),
    # Allow the user to set max_watts, defaulting to 810.0
    cv.Optional(CONF_MAX_WATTS, default=810.0): cv.float_,
}).extend(cv.polling_component_schema('1s'))

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    
    # Pass the value from YAML to the C++ setter
    cg.add(var.set_max_watts(config[CONF_MAX_WATTS]))
