import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor

from esphome.const import (
    CONF_ID,
    ICON_FAN,
    ICON_LIGHTBULB,
)

from .. import (
    CONF_SPA_ID,    
    balboa_spa_ns,
    BalboaSpa,
)

DEPENDENCIES = ["balboa_spa"]
AUTO_LOAD = ["binary_sensor"]

BalboaSpaSensors = balboa_spa_ns.class_('BalboaSpaSensors', cg.Component)

CONF_JET1 = "jet1"
CONF_JET2 = "jet2"
CONF_LIGHTS = "lights"

SENSOR_TYPES = {
    CONF_JET1: binary_sensor.binary_sensor_schema(
        icon=ICON_FAN,
    ),
    CONF_JET2: binary_sensor.binary_sensor_schema(
        icon=ICON_FAN,
    ),
    CONF_LIGHTS: binary_sensor.binary_sensor_schema(
        icon=ICON_LIGHTBULB,
    ),
}

CONFIG_SCHEMA = cv.Schema(
    {        
        cv.GenerateID(): cv.declare_id(BalboaSpaSensors),
        cv.GenerateID(CONF_SPA_ID): cv.use_id(BalboaSpa),
    }
).extend({cv.Optional(type): schema for type, schema in SENSOR_TYPES.items()})



async def to_code(config):    
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    parent = await cg.get_variable(config[CONF_SPA_ID])
    cg.add(var.set_parent(parent))

    if conf := config.get(CONF_JET1):
        sens = await binary_sensor.new_binary_sensor(conf)
        cg.add(var.set_jet1_binary_sensor(sens))
    
    if conf := config.get(CONF_JET2):
        sens = await binary_sensor.new_binary_sensor(conf)
        cg.add(var.set_jet2_binary_sensor(sens))

    if conf := config.get(CONF_LIGHTS):
        sens = await binary_sensor.new_binary_sensor(conf)
        cg.add(var.set_lights_binary_sensor(sens))
    