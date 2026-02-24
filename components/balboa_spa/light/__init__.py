import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import light

from .. import (
    balboa_spa_ns,
    BalboaSpa,
    CONF_SPA_ID
)

DEPENDENCIES = ["balboa_spa"]

LightsLight = balboa_spa_ns.class_("LightsLight", light.LightOutput)
Light2Light = balboa_spa_ns.class_("Light2Light", light.LightOutput)

CONF_LIGHT = "light"
CONF_LIGHT2 = "light2"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_SPA_ID): cv.use_id(BalboaSpa),
        cv.Optional(CONF_LIGHT): light.light_schema(LightsLight, light.LightType.BINARY),
        cv.Optional(CONF_LIGHT2): light.light_schema(Light2Light, light.LightType.BINARY),
    })

async def to_code(config):
    parent = await cg.get_variable(config[CONF_SPA_ID])

    for light_key in [CONF_LIGHT, CONF_LIGHT2]:
        if conf := config.get(light_key):
            output_var = await light.new_light(conf)
            cg.add(output_var.set_parent(parent))
