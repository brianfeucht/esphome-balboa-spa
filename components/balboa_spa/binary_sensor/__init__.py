import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor

from .. import (
    balboa_spa_ns,
    BalboaSpa,
    CONF_SPA_ID
)

from esphome.const import (
    DEVICE_CLASS_CONNECTIVITY,
    ENTITY_CATEGORY_DIAGNOSTIC,
    DEVICE_CLASS_POWER
)

DEPENDENCIES = ["balboa_spa"]

SpaSensor = balboa_spa_ns.class_("BalboaSpaBinarySensors", binary_sensor.BinarySensor)
SpaSensorTypeEnum = SpaSensor.enum("BalboaSpaBinarySensorType", True)

CONF_BLOWER = "blower"
CONF_HIGHRANGE = "highrange"
CONF_CIRCULATION = "circulation"
CONF_RESTMODE = "restmode"
CONF_HEATSTATE = "heatstate"
CONF_CONNECTED = "connected"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_SPA_ID): cv.use_id(BalboaSpa),
        cv.Optional(CONF_BLOWER): binary_sensor.binary_sensor_schema(
            SpaSensor,
            device_class=DEVICE_CLASS_POWER,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC
        ),
        cv.Optional(CONF_HIGHRANGE): binary_sensor.binary_sensor_schema(
            SpaSensor,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC
        ),
        cv.Optional(CONF_CIRCULATION): binary_sensor.binary_sensor_schema(
            SpaSensor,
            device_class=DEVICE_CLASS_POWER,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC
        ),
        cv.Optional(CONF_RESTMODE): binary_sensor.binary_sensor_schema(
            SpaSensor,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC
        ),
        cv.Optional(CONF_HEATSTATE): binary_sensor.binary_sensor_schema(
            SpaSensor,
            device_class=DEVICE_CLASS_POWER,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC
        ),
        cv.Optional(CONF_CONNECTED): binary_sensor.binary_sensor_schema(
            SpaSensor,
            icon="mdi:lan-connect",
            device_class=DEVICE_CLASS_CONNECTIVITY,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC
        ),
    })

async def to_code(config):
    parent = await cg.get_variable(config[CONF_SPA_ID])

    for sensor_type in [CONF_BLOWER, CONF_HIGHRANGE, CONF_CIRCULATION, CONF_RESTMODE, CONF_HEATSTATE, CONF_CONNECTED]:
        if conf := config.get(sensor_type):
            var = await binary_sensor.new_binary_sensor(conf)
            cg.add(var.set_parent(parent))
            sensor_type_value = getattr(SpaSensorTypeEnum, sensor_type.upper())
            cg.add(var.set_sensor_type(sensor_type_value))
