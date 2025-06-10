import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome.const import CONF_ID

DEPENDENCIES = ['uart']

CONF_SPA_ID = "balboa_spa_id"
CONF_POOL_SIZE = "pool_size"
CONF_POOL_THRESHOLD = "pool_stable_threshold"

balboa_spa_ns = cg.esphome_ns.namespace('balboa_spa')
BalboaSpa = balboa_spa_ns.class_('BalboaSpa', cg.Component, uart.UARTDevice)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(BalboaSpa),
        cv.Optional(CONF_POOL_SIZE): cv.int_range(min=1, max=20),
        cv.Optional(CONF_POOL_THRESHOLD): cv.int_range(min=1, max=20),
    }
).extend(cv.COMPONENT_SCHEMA).extend(uart.UART_DEVICE_SCHEMA)

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])

    yield cg.register_component(var, config)
    yield uart.register_uart_device(var, config)

    if conf := config.get(CONF_POOL_SIZE):
        cg.add(var.set_state_pool_size(conf))

    if conf := config.get(CONF_POOL_THRESHOLD):
        cg.add(var.set_state_pool_stable_threshold(conf))

    cg.add_library(
        name="CircularBuffer",
        repository="https://github.com/rlogiacco/CircularBuffer",
        version="1.4.0"
    )
