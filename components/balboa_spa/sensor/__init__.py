import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor

from .. import (
    balboa_spa_ns,
    BalboaSpa,
    CONF_SPA_ID
)

from esphome.const import (
    ICON_THERMOMETER,
    DEVICE_CLASS_CONNECTIVITY,
    ENTITY_CATEGORY_DIAGNOSTIC,
    DEVICE_CLASS_TEMPERATURE,
    STATE_CLASS_MEASUREMENT
)

DEPENDENCIES = ["balboa_spa"]

SpaSensor = balboa_spa_ns.class_("BalboaSpaSensors", sensor.Sensor)
SpaSensorTypeEnum = SpaSensor.enum("BalboaSpaSensorType", True)

CONF_BLOWER = "blower"
CONF_HIGHRANGE = "highrange"
CONF_CIRCULATION = "circulation"
CONF_RESTMODE = "restmode"
CONF_HEATSTATE = "heatstate"
CONF_CONNECTED = "connected"
CONF_SENSOR_A = "sensor_a"
CONF_SENSOR_B = "sensor_b"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_SPA_ID): cv.use_id(BalboaSpa),
        cv.Optional(CONF_BLOWER): sensor.sensor_schema(
            SpaSensor,
        ),
        cv.Optional(CONF_HIGHRANGE): sensor.sensor_schema(
            SpaSensor,
        ),
        cv.Optional(CONF_CIRCULATION): sensor.sensor_schema(
            SpaSensor,
        ),
        cv.Optional(CONF_RESTMODE): sensor.sensor_schema(
            SpaSensor,
        ),
        cv.Optional(CONF_HEATSTATE): sensor.sensor_schema(
            SpaSensor,
        ),
        cv.Optional(CONF_CONNECTED): sensor.sensor_schema(
            SpaSensor,
            icon="mdi:lan-connect",
            device_class=DEVICE_CLASS_CONNECTIVITY,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC
        ),
        cv.Optional(CONF_SENSOR_A): sensor.sensor_schema(
            SpaSensor,
            icon=ICON_THERMOMETER,
            device_class=DEVICE_CLASS_TEMPERATURE,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            state_class=STATE_CLASS_MEASUREMENT
        ),
        cv.Optional(CONF_SENSOR_B): sensor.sensor_schema(
            SpaSensor,
            icon=ICON_THERMOMETER,
            device_class=DEVICE_CLASS_TEMPERATURE,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            state_class=STATE_CLASS_MEASUREMENT
        ),
    })

async def to_code(config):
    parent = await cg.get_variable(config[CONF_SPA_ID])

    for sensor_type in [CONF_BLOWER, CONF_HIGHRANGE, CONF_CIRCULATION, CONF_RESTMODE, CONF_HEATSTATE, CONF_CONNECTED, CONF_SENSOR_A, CONF_SENSOR_B]:
        if conf := config.get(sensor_type):
            var = await sensor.new_sensor(conf)
            cg.add(var.set_parent(parent))
            sensor_type_value = getattr(SpaSensorTypeEnum, sensor_type.upper())
            cg.add(var.set_sensor_type(sensor_type_value))
