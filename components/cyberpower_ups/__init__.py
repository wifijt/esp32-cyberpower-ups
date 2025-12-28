import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID
from esphome.const import __version__ as esphome_version
if cv.Version.parse(esphome_version) < cv.Version.parse("2025.12.0"):
    raise cv.Invalid("This component requires ESPHome 2025.12.0 or newer")

CODEOWNERS = ["@wifijt"]
PROJECT_URL = "https://github.com/wifijt/esp32-cyberpower-ups"
VERSION = "1.1.0"

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
