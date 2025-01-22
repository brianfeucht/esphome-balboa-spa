import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import climate
from esphome.const import CONF_ID

from .. import (
    CONF_SPA_ID,
    balboa_spa_ns,
    BalboaSpa,
)

DEPENDENCIES = ["balboa_spa"]
AUTO_LOAD = ["climate"]

BalboaSpaThermostat = balboa_spa_ns.class_('BalboaSpaThermostat', cg.Component, climate.Climate)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(BalboaSpaThermostat),
        cv.GenerateID(CONF_SPA_ID): cv.use_id(BalboaSpa),
    }
).extend({cv.Optional("balboa_spa"):climate.CLIMATE_SCHEMA.extend(cv.COMPONENT_SCHEMA)})

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    parent = await cg.get_variable(config[CONF_SPA_ID])
    cg.add(var.set_parent(parent))

