import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import water_heater
from esphome.const import CONF_ID

from .. import (
    CONF_SPA_ID,
    balboa_spa_ns,
    BalboaSpa,
)

DEPENDENCIES = ["balboa_spa", "water_heater"]
AUTO_LOAD = ["water_heater"]

BalboaSpaWaterHeater = balboa_spa_ns.class_("BalboaSpaWaterHeater", water_heater.WaterHeater)

CONFIG_SCHEMA = (
    water_heater.water_heater_schema(BalboaSpaWaterHeater).extend(
        {
            cv.GenerateID(): cv.declare_id(BalboaSpaWaterHeater),
            cv.GenerateID(CONF_SPA_ID): cv.use_id(BalboaSpa),
        }
    )
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await water_heater.register_water_heater(var, config)

    parent = await cg.get_variable(config[CONF_SPA_ID])
    cg.add(var.set_parent(parent))
