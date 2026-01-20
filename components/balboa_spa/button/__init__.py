import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import button
from .. import balboa_spa_ns, BalboaSpa, CONF_SPA_ID

DEPENDENCIES = ["balboa_spa"]

SyncTimeButton = balboa_spa_ns.class_("SyncTimeButton", button.Button)
DisableFilter2Button = balboa_spa_ns.class_("DisableFilter2Button", button.Button)
RequestFaultLogButton = balboa_spa_ns.class_("RequestFaultLogButton", button.Button)

CONF_SYNC_TIME = "sync_time"
CONF_DISABLE_FILTER2 = "disable_filter2"
CONF_REQUEST_FAULT_LOG = "request_fault_log"

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(CONF_SPA_ID): cv.use_id(BalboaSpa),
    cv.Optional(CONF_SYNC_TIME): button.button_schema(SyncTimeButton),
    cv.Optional(CONF_DISABLE_FILTER2): button.button_schema(DisableFilter2Button),
    cv.Optional(CONF_REQUEST_FAULT_LOG): button.button_schema(RequestFaultLogButton),
})

async def to_code(config):
    parent = await cg.get_variable(config[CONF_SPA_ID])
    if conf := config.get(CONF_SYNC_TIME):
        var = await button.new_button(conf)
        cg.add(var.set_parent(parent))
    if conf := config.get(CONF_DISABLE_FILTER2):
        var = await button.new_button(conf)
        cg.add(var.set_parent(parent))
    if conf := config.get(CONF_REQUEST_FAULT_LOG):
        var = await button.new_button(conf)
        cg.add(var.set_parent(parent))
