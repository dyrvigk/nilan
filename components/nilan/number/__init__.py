import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import number
from .. import Nilan, CONF_NILAN_ID
from esphome.const import (
    CONF_ID,
    CONF_SENSOR,
    CONF_DEVICE_CLASS,
    CONF_ICON,
    UNIT_EMPTY,
    ICON_CHECK_CIRCLE_OUTLINE,
    DEVICE_CLASS_EMPTY,
    CONF_OPTIONS
)
 
CONF_USER_TIME_SET = "user_time_set"
CONF_USER_VENT_SET = "user_vent_set"
CONF_USER_TEMP_SET = "user_temp_set"
CONF_USER_TEMP_OFFSET_SET = "user_temp_offset_set"

nilan_ns = cg.esphome_ns.namespace('nilan')
NilanNumber = nilan_ns.class_('NilanNumber', number.Number, cg.Component)
 
CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(NilanNumber),
    cv.GenerateID(CONF_NILAN_ID): cv.use_id(Nilan),
    cv.Optional(CONF_USER_TIME_SET): number.NUMBER_SCHEMA,
    cv.Optional(CONF_USER_VENT_SET): number.NUMBER_SCHEMA,
    cv.Optional(CONF_USER_TEMP_SET): number.NUMBER_SCHEMA,
    cv.Optional(CONF_USER_TEMP_OFFSET_SET): number.NUMBER_SCHEMA,
}).extend(cv.COMPONENT_SCHEMA)
 
def to_code(config):
    nilan = yield cg.get_variable(config[CONF_NILAN_ID])
    var = cg.new_Pvariable(config[CONF_ID], nilan)

    if CONF_USER_TIME_SET in config:
        conf = config[CONF_USER_TIME_SET]
        yield cg.register_component(var, conf)
        yield number.register_number(var, conf, min_value=0, max_value=480, step=1.0)
    if CONF_USER_VENT_SET in config:
        conf = config[CONF_USER_VENT_SET]
        yield cg.register_component(var, conf)
        yield number.register_number(var, conf, min_value=0, max_value=4, step=1.0)
    if CONF_USER_TEMP_SET in config:
        conf = config[CONF_USER_TEMP_SET]
        yield cg.register_component(var, conf)
        yield number.register_number(var, conf, min_value=0, max_value=30, step=1.0)
    if CONF_USER_TEMP_OFFSET_SET in config:
        conf = config[CONF_USER_TEMP_OFFSET_SET]
        yield cg.register_component(var, conf)
        yield number.register_number(var, conf, min_value=0, max_value=30, step=1.0)