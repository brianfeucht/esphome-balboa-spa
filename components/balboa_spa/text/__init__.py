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
SpaFilter1ConfigText = balboa_spa_ns.class_("SpaFilter1ConfigText", text.Text)
SpaFilter2ConfigText = balboa_spa_ns.class_("SpaFilter2ConfigText", text.Text)

CONF_SPA_TIME = "spa_time"
CONF_FILTER1_CONFIG = "filter1_config"
CONF_FILTER2_CONFIG = "filter2_config"

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(CONF_SPA_ID): cv.use_id(BalboaSpa),
    cv.Optional(CONF_SPA_TIME): text.text_schema(
        SpaTimeText,
        icon=ICON_TIMER,
    ),
    cv.Optional(CONF_FILTER1_CONFIG): text.text_schema(
        SpaFilter1ConfigText,
        icon=ICON_WATER,
    ),
    cv.Optional(CONF_FILTER2_CONFIG): text.text_schema(
        SpaFilter2ConfigText,
        icon=ICON_WATER,
    ),
})

async def to_code(config):
    parent = await cg.get_variable(config[CONF_SPA_ID])

    if conf := config.get(CONF_SPA_TIME):
        var = await text.new_text(conf, SpaTimeText)
        cg.add(var.set_parent(parent))

    if conf := config.get(CONF_FILTER1_CONFIG):
        var = await text.new_text(conf, SpaFilter1ConfigText)
        cg.add(var.set_parent(parent))

    if conf := config.get(CONF_FILTER2_CONFIG):
        var = await text.new_text(conf, SpaFilter2ConfigText)
        cg.add(var.set_parent(parent))