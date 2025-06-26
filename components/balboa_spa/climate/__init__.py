import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import climate
from esphome.const import CONF_ID

from .. import (
    CONF_SPA_ID,
    balboa_spa_ns,
    BalboaSpa,
)

DEPENDENCIES = ["balboa_spa", "climate"]
AUTO_LOAD = ["climate"]

BalboaSpaThermostat = balboa_spa_ns.class_('BalboaSpaThermostat', cg.Component, climate.Climate)

SHOW_AS_F = "as_f"

CONFIG_SCHEMA = (
    climate.climate_schema(BalboaSpaThermostat).extend(
    {
        cv.GenerateID(): cv.declare_id(BalboaSpaThermostat),
        cv.GenerateID(CONF_SPA_ID): cv.use_id(BalboaSpa),
        cv.Optional(SHOW_AS_F, default=False): cv.boolean,
    })
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    
    await climate.register_climate(var, config)

    parent = await cg.get_variable(config[CONF_SPA_ID])
    cg.add(var.set_parent(parent))
    cg.add(var.set_is_f(config[SHOW_AS_F]))
