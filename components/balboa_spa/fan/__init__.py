import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import fan
from esphome.const import ICON_FAN
from .. import CONF_SPA_ID, BalboaSpa, balboa_spa_ns

DEPENDENCIES = ["balboa_spa"]

JetFan = balboa_spa_ns.class_("JetFan", fan.Fan, cg.Component)

CONF_JET_1 = "jet_1"
CONF_JET_2 = "jet_2"
CONF_JET_3 = "jet_3"
CONF_JET_4 = "jet_4"

def jet_fan_schema():
    return fan.fan_schema(JetFan, icon=ICON_FAN)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_SPA_ID): cv.use_id(BalboaSpa),
        cv.Optional(CONF_JET_1): jet_fan_schema(),
        cv.Optional(CONF_JET_2): jet_fan_schema(),
        cv.Optional(CONF_JET_3): jet_fan_schema(),
        cv.Optional(CONF_JET_4): jet_fan_schema(),
    }
)

async def to_code(config):
    parent = await cg.get_variable(config[CONF_SPA_ID])

    jet_configs = [
        (CONF_JET_1, 1, "Jet1"),
        (CONF_JET_2, 2, "Jet2"),
        (CONF_JET_3, 3, "Jet3"),
        (CONF_JET_4, 4, "Jet4"),
    ]

    for conf_key, jet_number, jet_name in jet_configs:
        if conf := config.get(conf_key):
            fan_var = await fan.new_fan(conf)
            cg.add(fan_var.set_parent(parent))
            cg.add(fan_var.set_jet_number(jet_number))
            cg.add(fan_var.set_jet_name(jet_name))
            cg.add(parent.register_listener(fan_var.get_listener()))