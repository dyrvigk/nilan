import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import select
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
 
CONF_SET_USER_FUNCTION = "set_user_function"

nilan_ns = cg.esphome_ns.namespace('nilan')
NilanSelect = nilan_ns.class_('NilanSelect', select.Select, cg.Component)
 
CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(NilanSelect),
    cv.GenerateID(CONF_NILAN_ID): cv.use_id(Nilan),
    cv.Required(CONF_SET_USER_FUNCTION): select.SELECT_SCHEMA.extend({
        cv.Required(CONF_OPTIONS): cv.All(
            cv.ensure_list(cv.string_strict), cv.Length(min=1)
        ),
    })
}).extend(cv.COMPONENT_SCHEMA)
 
def to_code(config):
    nilan = yield cg.get_variable(config[CONF_NILAN_ID])
    var = cg.new_Pvariable(config[CONF_ID], nilan)

    if CONF_SET_USER_FUNCTION in config:
        conf = config[CONF_SET_USER_FUNCTION]
        yield cg.register_component(var, conf)
        yield select.register_select(var, conf, options=conf[CONF_OPTIONS])