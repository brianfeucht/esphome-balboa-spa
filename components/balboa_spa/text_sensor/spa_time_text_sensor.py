import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import text_sensor
from .. import balboa_spa_ns, BalboaSpa, CONF_SPA_ID

DEPENDENCIES = ["balboa_spa"]
SpaTimeTextSensor = balboa_spa_ns.class_("SpaTimeTextSensor", text_sensor.TextSensor)

CONF_SPA_TIME = "spa_time"

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(CONF_SPA_ID): cv.use_id(BalboaSpa),
    cv.Optional(CONF_SPA_TIME): text_sensor.text_sensor_schema(SpaTimeTextSensor),
})

async def to_code(config):
    parent = await cg.get_variable(config[CONF_SPA_ID])
    if conf := config.get(CONF_SPA_TIME):
        var = await text_sensor.new_text_sensor(conf)
        cg.add(var.set_parent(parent))
