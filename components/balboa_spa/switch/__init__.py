import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch

from esphome.const import (
    ICON_FAN,
    ICON_LIGHTBULB,
    ICON_GRAIN,
    ICON_THERMOMETER,
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
Light2Switch = balboa_spa_ns.class_("Light2Switch", switch.Switch)
BlowerSwitch = balboa_spa_ns.class_("BlowerSwitch", switch.Switch)
HighrangeSwitch = balboa_spa_ns.class_("HighrangeSwitch", switch.Switch)

CONF_JET1 = "jet1"
CONF_JET2 = "jet2"
CONF_JET3 = "jet3"
CONF_JET4 = "jet4"
CONF_LIGHTS = "light"
CONF_LIGHT2 = "light2"
CONF_BLOWER = "blower"
CONF_HIGHRANGE = "highrange"
CONF_DISCARD_UPDATES = "discard_updates"  
CONF_MAX_TOGGLE_ATTEMPTS = "max_toggle_attempts"

def jet_switch_schema(cls):
    return switch.switch_schema(
        cls,
        icon=ICON_FAN,
        default_restore_mode="DISABLED",
    ).extend({
        cv.Optional(CONF_MAX_TOGGLE_ATTEMPTS, default=5): cv.positive_int,
        cv.Optional(CONF_DISCARD_UPDATES, default=20): cv.positive_int,
    })

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_SPA_ID): cv.use_id(BalboaSpa),
        cv.Optional(CONF_JET1): jet_switch_schema(Jet1Switch),
        cv.Optional(CONF_JET2): jet_switch_schema(Jet2Switch),
        cv.Optional(CONF_JET3): jet_switch_schema(Jet3Switch),
        cv.Optional(CONF_JET4): jet_switch_schema(Jet4Switch),
        cv.Optional(CONF_LIGHTS): switch.switch_schema(
            LightsSwitch,
            icon=ICON_LIGHTBULB,
            default_restore_mode="DISABLED",
        ),
        cv.Optional(CONF_LIGHT2): switch.switch_schema(
            Light2Switch,
            icon=ICON_LIGHTBULB,
            default_restore_mode="DISABLED",
        ),
        cv.Optional(CONF_BLOWER): switch.switch_schema(
            BlowerSwitch,
            icon=ICON_GRAIN,
            default_restore_mode="DISABLED",
        ).extend({
            cv.Optional(CONF_DISCARD_UPDATES, default=20): cv.positive_int,
        }),
        cv.Optional(CONF_HIGHRANGE): switch.switch_schema(
            HighrangeSwitch,
            icon=ICON_THERMOMETER,
            default_restore_mode="DISABLED",
        ),
    })

async def to_code(config):
    parent = await cg.get_variable(config[CONF_SPA_ID])

    for switch_type, cls in [
        (CONF_JET1, Jet1Switch),
        (CONF_JET2, Jet2Switch),
        (CONF_JET3, Jet3Switch),
        (CONF_JET4, Jet4Switch),
        (CONF_BLOWER, BlowerSwitch),
        (CONF_LIGHTS, LightsSwitch),
        (CONF_LIGHT2, Light2Switch),
        (CONF_HIGHRANGE, HighrangeSwitch),
    ]:
        if conf := config.get(switch_type):
            sw_var = await switch.new_switch(conf)
            cg.add(sw_var.set_parent(parent))
            if CONF_MAX_TOGGLE_ATTEMPTS in conf:
                cg.add(sw_var.set_max_toggle_attempts(conf[CONF_MAX_TOGGLE_ATTEMPTS]))
            if CONF_DISCARD_UPDATES in conf:
                cg.add(sw_var.set_discard_updates(conf[CONF_DISCARD_UPDATES]))
