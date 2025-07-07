import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import fan
from esphome.const import ICON_FAN
from .. import (
    balboa_spa_ns,
    BalboaSpa,
    CONF_SPA_ID
)

DEPENDENCIES = ["balboa_spa"]

Jet1Fan = balboa_spa_ns.class_("Jet1Fan", fan.Fan)
Jet2Fan = balboa_spa_ns.class_("Jet2Fan", fan.Fan)
Jet3Fan = balboa_spa_ns.class_("Jet3Fan", fan.Fan)
Jet4Fan = balboa_spa_ns.class_("Jet4Fan", fan.Fan)

CONF_JET1 = "jet1"
CONF_JET2 = "jet2"
CONF_JET3 = "jet3"
CONF_JET4 = "jet4"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_SPA_ID): cv.use_id(BalboaSpa),
        cv.Optional(CONF_JET1): fan.fan_schema(
            Jet1Fan,
            icon=ICON_FAN,
        ).extend({
            cv.Optional("speed_count", default=3): cv.int_range(min=3, max=3),
        }),
        cv.Optional(CONF_JET2): fan.fan_schema(
            Jet2Fan,
            icon=ICON_FAN,
        ).extend({
            cv.Optional("speed_count", default=3): cv.int_range(min=3, max=3),
        }),
        cv.Optional(CONF_JET3): fan.fan_schema(
            Jet3Fan,
            icon=ICON_FAN,
        ).extend({
            cv.Optional("speed_count", default=3): cv.int_range(min=3, max=3),
        }),
        cv.Optional(CONF_JET4): fan.fan_schema(
            Jet4Fan,
            icon=ICON_FAN,
        ).extend({
            cv.Optional("speed_count", default=3): cv.int_range(min=3, max=3),
        }),
    })

async def to_code(config):
    parent = await cg.get_variable(config[CONF_SPA_ID])
    
    if CONF_JET1 in config:
        conf = config[CONF_JET1]
        var = await fan.new_fan(conf, Jet1Fan())
        await cg.register_component(var, conf)
        cg.add(var.set_parent(parent))

    if CONF_JET2 in config:
        conf = config[CONF_JET2]
        var = await fan.new_fan(conf, Jet2Fan())
        await cg.register_component(var, conf)
        cg.add(var.set_parent(parent))

    if CONF_JET3 in config:
        conf = config[CONF_JET3]
        var = await fan.new_fan(conf, Jet3Fan())
        await cg.register_component(var, conf)
        cg.add(var.set_parent(parent))

    if CONF_JET4 in config:
        conf = config[CONF_JET4]
        var = await fan.new_fan(conf, Jet4Fan())
        await cg.register_component(var, conf)
        cg.add(var.set_parent(parent))
