import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import number
from .. import Nilan, CONF_NILAN_ID
from esphome.const import (
    CONF_ID,
    DEVICE_CLASS_EMPTY,
    CONF_MIN_VALUE,
    CONF_MAX_VALUE,
    CONF_STEP,
    CONF_NAME,
    CONF_TYPE,
)
 
CONF_USER_TIME_SET = "user_time_set"
CONF_USER_VENT_SET = "user_vent_set"
CONF_USER_TEMP_SET = "user_temp_set"
CONF_USER_TEMP_OFFSET_SET = "user_temp_offset_set"
CONF_NUMBERS = "numbers"

nilan_ns = cg.esphome_ns.namespace('nilan')
NilanNumber = nilan_ns.class_('NilanNumber', number.Number, cg.Component)
NilanNumberType = nilan_ns.enum("NilanNumberType", is_class=True)

NUMBER_TYPES = {
    1: NilanNumberType.USER_TEMP_SET,
    2: NilanNumberType.USER_VENT_SET,
    3: NilanNumberType.USER_TIME_SET,
}

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(NilanNumber),
        cv.GenerateID(CONF_NILAN_ID): cv.use_id(Nilan),
        cv.Optional(
            CONF_NUMBERS,
            default=[
                {
                    CONF_NAME: "User temperature set",
                    CONF_TYPE: 1,
                    CONF_MIN_VALUE: 15,
                    CONF_MAX_VALUE: 30,
                    CONF_STEP: 1,
                },
                {
                    CONF_NAME: "User ventilation speed set",
                    CONF_TYPE: 2,
                    CONF_MIN_VALUE: 0,
                    CONF_MAX_VALUE: 4,
                    CONF_STEP: 5,
                },
                {
                    CONF_NAME: "User time set (minutes)",
                    CONF_TYPE: 3,
                    CONF_MIN_VALUE: 0,
                    CONF_MAX_VALUE: 480,
                    CONF_STEP: 1,
                },
            ],
        ): [
            number.NUMBER_SCHEMA.extend(cv.COMPONENT_SCHEMA).extend(
                {
                    cv.GenerateID(): cv.declare_id(NilanNumber),
                    cv.Required(CONF_TYPE): cv.enum(NUMBER_TYPES, int=True),
                    cv.Required(CONF_MIN_VALUE): cv.float_,
                    cv.Required(CONF_MAX_VALUE): cv.float_,
                    cv.Required(CONF_STEP): cv.float_,
                }
            )
        ]
    }
)

def to_code(config):
    #nilan = yield cg.get_variable(config[CONF_NILAN_ID])
    for conf in config[CONF_NUMBERS]:
        var = cg.new_Pvariable(conf[CONF_ID])
        yield cg.register_component(var, conf)
        yield number.register_number(
            var,
            conf,
            min_value=conf[CONF_MIN_VALUE],
            max_value=conf[CONF_MAX_VALUE],
            step=conf[CONF_STEP],
        )
        cg.add(var.set_type(conf[CONF_TYPE]))