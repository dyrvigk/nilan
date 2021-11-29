import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import select, modbus_controller
from .. import Nilan, CONF_NILAN_ID

from esphome.components.modbus_controller import (
    CONF_MODBUS_CONTROLLER_ID,
    CONF_FORCE_NEW_RANGE,
    CONF_BYTE_OFFSET,
    CONF_SKIP_UPDATES,
    CONF_REGISTER_TYPE,
    CONF_VALUE_TYPE,
    SENSOR_VALUE_TYPE,
)

from esphome.const import (
    CONF_ID,
    CONF_ADDRESS,
    CONF_OPTIONS,
    CONF_OFFSET,
)

DEPENDENCIES = ["modbus_controller"]

nilan_ns = cg.esphome_ns.namespace('nilan')
NilanSelect = nilan_ns.class_('NilanSelect', select.Select, cg.Component)
 
CONFIG_SCHEMA = select.SELECT_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(NilanSelect),
    cv.GenerateID(CONF_NILAN_ID): cv.use_id(Nilan),
    cv.Required(CONF_ADDRESS): cv.positive_int,
    cv.Required(CONF_MODBUS_CONTROLLER_ID): cv.use_id(modbus_controller.ModbusController),
    cv.Required(CONF_OPTIONS): cv.All(cv.ensure_list(cv.string_strict), cv.Length(min=1)),
    cv.Optional(CONF_OFFSET, default=0): cv.positive_int,
    cv.Optional(CONF_BYTE_OFFSET): cv.positive_int,
    #cv.Optional(CONF_BITMASK, default=0xFFFFFFFF): cv.hex_uint32_t,
    cv.Optional(CONF_VALUE_TYPE, default="U_WORD"): cv.enum(SENSOR_VALUE_TYPE),
    #cv.Optional(CONF_REGISTER_COUNT, default=0): cv.positive_int,
    cv.Optional(CONF_SKIP_UPDATES, default=0): cv.positive_int,
    cv.Optional(CONF_FORCE_NEW_RANGE, default=False): cv.boolean,
    cv.Optional(CONF_FORCE_NEW_RANGE, default=False): cv.boolean,
}).extend(cv.COMPONENT_SCHEMA)
 
def to_code(config):
    byte_offset = 0
    if CONF_OFFSET in config:
        byte_offset = config[CONF_OFFSET]
    # A CONF_BYTE_OFFSET setting overrides CONF_OFFSET
    if CONF_BYTE_OFFSET in config:
        byte_offset = config[CONF_BYTE_OFFSET]
    value_type = config[CONF_VALUE_TYPE]
    bitmask = 0xFFFFFFFF
    reg_count = 1

    var = cg.new_Pvariable(
        config[CONF_ID],
        config[CONF_ADDRESS],
        byte_offset,
        bitmask,
        value_type,
        reg_count,
        config[CONF_SKIP_UPDATES],
        config[CONF_FORCE_NEW_RANGE],
    )

    yield cg.register_component(var, config)
    yield select.register_select(var, config, options=config[CONF_OPTIONS])

    modbus_controller = yield cg.get_variable(config[CONF_MODBUS_CONTROLLER_ID])
    cg.add(var.set_parent(modbus_controller))
    cg.add(modbus_controller.add_sensor_item(var))