import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import button
from .. import balboa_spa_ns, BalboaSpa, CONF_SPA_ID

DEPENDENCIES = ["balboa_spa"]

SyncTimeButton = balboa_spa_ns.class_("SyncTimeButton", button.Button)

CONF_SYNC_TIME = "sync_time"

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(CONF_SPA_ID): cv.use_id(BalboaSpa),
    cv.Optional(CONF_SYNC_TIME): button.button_schema(SyncTimeButton),
})

async def to_code(config):
    parent = await cg.get_variable(config[CONF_SPA_ID])
    if conf := config.get(CONF_SYNC_TIME):
        var = await button.new_button(conf)
        cg.add(var.set_parent(parent))
