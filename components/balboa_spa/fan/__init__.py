import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import fan
from esphome.const import ICON_FAN
from .. import CONF_SPA_ID, BalboaSpa, balboa_spa_ns

DEPENDENCIES = ["balboa_spa"]

Jet1Fan = balboa_spa_ns.class_("Jet1Fan", fan.Fan, cg.Component)
Jet2Fan = balboa_spa_ns.class_("Jet2Fan", fan.Fan, cg.Component)
Jet3Fan = balboa_spa_ns.class_("Jet3Fan", fan.Fan, cg.Component)
Jet4Fan = balboa_spa_ns.class_("Jet4Fan", fan.Fan, cg.Component)

CONF_JET_1 = "jet_1"
CONF_JET_2 = "jet_2"
CONF_JET_3 = "jet_3"
CONF_JET_4 = "jet_4"

def jet_fan_schema(cls):
    return fan.fan_schema(cls, icon=ICON_FAN)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_SPA_ID): cv.use_id(BalboaSpa),
        cv.Optional(CONF_JET_1): jet_fan_schema(Jet1Fan),
        cv.Optional(CONF_JET_2): jet_fan_schema(Jet2Fan),
        cv.Optional(CONF_JET_3): jet_fan_schema(Jet3Fan),
        cv.Optional(CONF_JET_4): jet_fan_schema(Jet4Fan),
    }
)

async def to_code(config):
    parent = await cg.get_variable(config[CONF_SPA_ID])

    for fan_type, cls in [
        (CONF_JET_1, Jet1Fan),
        (CONF_JET_2, Jet2Fan),
        (CONF_JET_3, Jet3Fan),
        (CONF_JET_4, Jet4Fan),
    ]:
        if conf := config.get(fan_type):
            fan_var = await fan.new_fan(conf)
            cg.add(fan_var.set_parent(parent))
            cg.add(parent.register_listener(fan_var.get_listener()))