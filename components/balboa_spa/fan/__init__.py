import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import fan

from esphome.const import (
    ICON_FAN,
)

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

CONF_JET_1 = "jet_1"
CONF_JET_2 = "jet_2"
CONF_JET_3 = "jet_3"
CONF_JET_4 = "jet_4"
CONF_MAX_TOGGLE_ATTEMPTS = "max_toggle_attempts"
CONF_DISCARD_UPDATES = "discard_updates"

def jet_fan_schema(cls):
    return fan.fan_schema(cls).extend({
        cv.Optional(CONF_MAX_TOGGLE_ATTEMPTS, default=5): cv.positive_int,
        cv.Optional(CONF_DISCARD_UPDATES, default=20): cv.positive_int,
    })

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_SPA_ID): cv.use_id(BalboaSpa),
        cv.Optional(CONF_JET_1): jet_fan_schema(Jet1Fan),
        cv.Optional(CONF_JET_2): jet_fan_schema(Jet2Fan),
        cv.Optional(CONF_JET_3): jet_fan_schema(Jet3Fan),
        cv.Optional(CONF_JET_4): jet_fan_schema(Jet4Fan),
    })

async def to_code(config):
    parent = await cg.get_variable(config[CONF_SPA_ID])

    for fan_type, cls in [
        (CONF_JET_1, Jet1Fan),
        (CONF_JET_2, Jet2Fan),
        (CONF_JET_3, Jet3Fan),
        (CONF_JET_4, Jet4Fan),
    ]:
        if conf := config.get(fan_type):
            fan_var = cg.new_Pvariable(conf[cv.CONF_ID])
            await fan.register_fan(fan_var, conf)
            cg.add(fan_var.set_parent(parent))
            if CONF_MAX_TOGGLE_ATTEMPTS in conf:
                cg.add(fan_var.set_max_toggle_attempts(conf[CONF_MAX_TOGGLE_ATTEMPTS]))
            if CONF_DISCARD_UPDATES in conf:
                cg.add(fan_var.set_discard_updates(conf[CONF_DISCARD_UPDATES]))
