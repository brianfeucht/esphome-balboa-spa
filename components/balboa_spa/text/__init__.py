import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import text
from esphome.const import ICON_TIMER, ICON_WATER

from .. import (
    balboa_spa_ns,
    BalboaSpa,
    CONF_SPA_ID
)

DEPENDENCIES = ["balboa_spa"]

SpaTimeText = balboa_spa_ns.class_("SpaTimeText", text.Text)
SpaFilter1StartTimeText = balboa_spa_ns.class_("SpaFilter1StartTimeText", text.Text)
SpaFilter1DurationText = balboa_spa_ns.class_("SpaFilter1DurationText", text.Text)
SpaFilter2StartTimeText = balboa_spa_ns.class_("SpaFilter2StartTimeText", text.Text)
SpaFilter2DurationText = balboa_spa_ns.class_("SpaFilter2DurationText", text.Text)

CONF_SPA_TIME = "spa_time"
CONF_FILTER1_START_TIME = "filter1_start_time"
CONF_FILTER1_DURATION = "filter1_duration"
CONF_FILTER2_START_TIME = "filter2_start_time"
CONF_FILTER2_DURATION = "filter2_duration"

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(CONF_SPA_ID): cv.use_id(BalboaSpa),
    cv.Optional(CONF_SPA_TIME): text.text_schema(
        SpaTimeText,
        icon=ICON_TIMER,
    ),
    cv.Optional(CONF_FILTER1_START_TIME): text.text_schema(
        SpaFilter1StartTimeText,
        icon=ICON_WATER,
    ),
    cv.Optional(CONF_FILTER1_DURATION): text.text_schema(
        SpaFilter1DurationText,
        icon=ICON_TIMER,
    ),
    cv.Optional(CONF_FILTER2_START_TIME): text.text_schema(
        SpaFilter2StartTimeText,
        icon=ICON_WATER,
    ),
    cv.Optional(CONF_FILTER2_DURATION): text.text_schema(
        SpaFilter2DurationText,
        icon=ICON_TIMER,
    ),
})

async def to_code(config):
    parent = await cg.get_variable(config[CONF_SPA_ID])

    if conf := config.get(CONF_SPA_TIME):
        var = await text.new_text(conf)
        cg.add(var.set_parent(parent))

    if conf := config.get(CONF_FILTER1_START_TIME):
        var = await text.new_text(conf)
        cg.add(var.set_parent(parent))

    if conf := config.get(CONF_FILTER1_DURATION):
        var = await text.new_text(conf)
        cg.add(var.set_parent(parent))

    if conf := config.get(CONF_FILTER2_START_TIME):
        var = await text.new_text(conf)
        cg.add(var.set_parent(parent))

    if conf := config.get(CONF_FILTER2_DURATION):
        var = await text.new_text(conf)
        cg.add(var.set_parent(parent))