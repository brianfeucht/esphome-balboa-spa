import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch

from esphome.const import (
    ICON_FAN,
    ICON_LIGHTBULB,
)

from .. import (
    balboa_spa_ns,
    BalboaSpa,
    CONF_SPA_ID
)

DEPENDENCIES = ["balboa_spa"]

Jet1Switch = balboa_spa_ns.class_("Jet1Switch", switch.Switch)
Jet2Switch = balboa_spa_ns.class_("Jet2Switch", switch.Switch)
LightsSwitch = balboa_spa_ns.class_("LightsSwitch", switch.Switch)

CONF_JET1 = "jet1"
CONF_JET2 = "jet2"
CONF_LIGHTS = "light"

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
        cv.Optional(CONF_LIGHTS): switch.switch_schema(
            LightsSwitch,
            icon=ICON_LIGHTBULB,
            default_restore_mode="DISABLED",
        ),
    })

async def to_code(config):
    parent = await cg.get_variable(config[CONF_SPA_ID])

    for switch_type in [CONF_JET1, CONF_JET2, CONF_LIGHTS]:
        if conf := config.get(switch_type):
            sw_var = await switch.new_switch(conf)
            # await cg.register_parented(sw_var, parent)
