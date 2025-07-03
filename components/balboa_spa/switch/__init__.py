import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch

from esphome.const import (
    ICON_FAN,
    ICON_LIGHTBULB,
    ICON_GRAIN,
)

from .. import (
    balboa_spa_ns,
    BalboaSpa,
    CONF_SPA_ID
)

DEPENDENCIES = ["balboa_spa"]

Jet1Switch = balboa_spa_ns.class_("Jet1Switch", switch.Switch)
Jet2Switch = balboa_spa_ns.class_("Jet2Switch", switch.Switch)
Jet3Switch = balboa_spa_ns.class_("Jet3Switch", switch.Switch)
Jet4Switch = balboa_spa_ns.class_("Jet4Switch", switch.Switch)
LightsSwitch = balboa_spa_ns.class_("LightsSwitch", switch.Switch)
BlowerSwitch = balboa_spa_ns.class_("BlowerSwitch", switch.Switch)

CONF_JET1 = "jet1"
CONF_JET2 = "jet2"
CONF_JET3 = "jet3"
CONF_JET4 = "jet4"
CONF_LIGHTS = "light"
CONF_BLOWER = "blower"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_SPA_ID): cv.use_id(BalboaSpa),
        cv.Optional(CONF_JET1): switch.switch_schema(
            Jet1Switch,
            icon=ICON_FAN,
            default_restore_mode="DISABLED",
        ),
        cv.Optional(CONF_JET2): switch.switch_schema(
            Jet2Switch,
            icon=ICON_FAN,
            default_restore_mode="DISABLED",
        ),
        cv.Optional(CONF_JET3): switch.switch_schema(
            Jet3Switch,
            icon=ICON_FAN,
            default_restore_mode="DISABLED",
        ),
        cv.Optional(CONF_JET4): switch.switch_schema(
            Jet4Switch,
            icon=ICON_FAN,
            default_restore_mode="DISABLED",
        ),
        cv.Optional(CONF_LIGHTS): switch.switch_schema(
            LightsSwitch,
            icon=ICON_LIGHTBULB,
            default_restore_mode="DISABLED",
        ),
        cv.Optional(CONF_BLOWER): switch.switch_schema(
            BlowerSwitch,
            icon=ICON_GRAIN,
            default_restore_mode="DISABLED",
        ),
    })

async def to_code(config):
    parent = await cg.get_variable(config[CONF_SPA_ID])

    for switch_type in [CONF_JET1, CONF_JET2, CONF_JET3, CONF_JET4, CONF_LIGHTS, CONF_BLOWER]:
        if conf := config.get(switch_type):
            sw_var = await switch.new_switch(conf)
            cg.add(sw_var.set_parent(parent))
