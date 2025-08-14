import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import number
from esphome.const import CONF_ID

from .. import (
    balboa_spa_ns,
    BalboaSpa,
    CONF_SPA_ID
)

DEPENDENCIES = ["balboa_spa"]

# Define number types
SpaHourNumber = balboa_spa_ns.class_("SpaHourNumber", number.Number)
SpaMinuteNumber = balboa_spa_ns.class_("SpaMinuteNumber", number.Number)
Filter1StartHourNumber = balboa_spa_ns.class_("Filter1StartHourNumber", number.Number)
Filter1StartMinuteNumber = balboa_spa_ns.class_("Filter1StartMinuteNumber", number.Number)
Filter1DurationHourNumber = balboa_spa_ns.class_("Filter1DurationHourNumber", number.Number)
Filter1DurationMinuteNumber = balboa_spa_ns.class_("Filter1DurationMinuteNumber", number.Number)
Filter2StartHourNumber = balboa_spa_ns.class_("Filter2StartHourNumber", number.Number)
Filter2StartMinuteNumber = balboa_spa_ns.class_("Filter2StartMinuteNumber", number.Number)  
Filter2DurationHourNumber = balboa_spa_ns.class_("Filter2DurationHourNumber", number.Number)
Filter2DurationMinuteNumber = balboa_spa_ns.class_("Filter2DurationMinuteNumber", number.Number)

# Configuration keys
CONF_SPA_HOUR = "spa_hour"
CONF_SPA_MINUTE = "spa_minute"
CONF_FILTER1_START_HOUR = "filter1_start_hour"
CONF_FILTER1_START_MINUTE = "filter1_start_minute"
CONF_FILTER1_DURATION_HOUR = "filter1_duration_hour"
CONF_FILTER1_DURATION_MINUTE = "filter1_duration_minute"
CONF_FILTER2_START_HOUR = "filter2_start_hour"
CONF_FILTER2_START_MINUTE = "filter2_start_minute"
CONF_FILTER2_DURATION_HOUR = "filter2_duration_hour"
CONF_FILTER2_DURATION_MINUTE = "filter2_duration_minute"

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(CONF_SPA_ID): cv.use_id(BalboaSpa),
    cv.Optional(CONF_SPA_HOUR): number.number_schema(SpaHourNumber),
    cv.Optional(CONF_SPA_MINUTE): number.number_schema(SpaMinuteNumber),
    cv.Optional(CONF_FILTER1_START_HOUR): number.number_schema(Filter1StartHourNumber),
    cv.Optional(CONF_FILTER1_START_MINUTE): number.number_schema(Filter1StartMinuteNumber),
    cv.Optional(CONF_FILTER1_DURATION_HOUR): number.number_schema(Filter1DurationHourNumber),
    cv.Optional(CONF_FILTER1_DURATION_MINUTE): number.number_schema(Filter1DurationMinuteNumber),
    cv.Optional(CONF_FILTER2_START_HOUR): number.number_schema(Filter2StartHourNumber),
    cv.Optional(CONF_FILTER2_START_MINUTE): number.number_schema(Filter2StartMinuteNumber),
    cv.Optional(CONF_FILTER2_DURATION_HOUR): number.number_schema(Filter2DurationHourNumber),
    cv.Optional(CONF_FILTER2_DURATION_MINUTE): number.number_schema(Filter2DurationMinuteNumber),
})

async def to_code(config):
    parent = await cg.get_variable(config[CONF_SPA_ID])

    number_configs = [
        (CONF_SPA_HOUR, SpaHourNumber, 0, 23, 1),
        (CONF_SPA_MINUTE, SpaMinuteNumber, 0, 59, 1),
        (CONF_FILTER1_START_HOUR, Filter1StartHourNumber, 0, 23, 1),
        (CONF_FILTER1_START_MINUTE, Filter1StartMinuteNumber, 0, 59, 1),
        (CONF_FILTER1_DURATION_HOUR, Filter1DurationHourNumber, 0, 23, 1),
        (CONF_FILTER1_DURATION_MINUTE, Filter1DurationMinuteNumber, 0, 59, 1),
        (CONF_FILTER2_START_HOUR, Filter2StartHourNumber, 0, 23, 1),
        (CONF_FILTER2_START_MINUTE, Filter2StartMinuteNumber, 0, 59, 1),
        (CONF_FILTER2_DURATION_HOUR, Filter2DurationHourNumber, 0, 23, 1),
        (CONF_FILTER2_DURATION_MINUTE, Filter2DurationMinuteNumber, 0, 59, 1),
    ]

    for conf_key, number_class, min_val, max_val, step_val in number_configs:
        if conf := config.get(conf_key):
            num_var = await number.new_number(
                conf,
                min_value=min_val,
                max_value=max_val,
                step=step_val
            )
            cg.add(num_var.set_parent(parent))