import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
from .. import Nilan, CONF_NILAN_ID
from esphome.const import (
    CONF_ID,
    CONF_SENSOR,
    CONF_DEVICE_CLASS,
    CONF_ICON,
    UNIT_EMPTY,
    ICON_CHECK_CIRCLE_OUTLINE,
    DEVICE_CLASS_EMPTY
)
 
CONF_IS_SUMMER = "is_summer"
CONF_FILTER_OK = "filter_ok"
CONF_BYPASS_ON_OFF = "bypass_on_off"
CONF_DOOR_OPEN = "door_open"
CONF_AIRFLAP = "airflap"
CONF_SMOKEFLAP = "smokeflap"
CONF_AIRCIRC_PUMP = "aircirculation_pump"
CONF_AIRHEAT_ALLOW = "airheat_allow"
CONF_AIRHEAT_1 = "airheat_1"
CONF_AIRHEAT_2 = "airheat_2"
CONF_AIRHEAT_3 = "airheat_3"
CONF_COMPRESSOR = "compressor"
CONF_COMPRESSOR_2 = "compressor_2"
CONF_FOUR_WAY_COOL = "four_way_cool"
CONF_HOTGAS_HEAT = "hotgas_heat"
CONF_HOTGAS_COOL = "hotgas_cool"
CONF_CONDENSER_OPEN = "condenser_open"
CONF_CONDENSER_CLOSE = "condenser_close"
CONF_WATER_HEAT = "water_heat"
CONF_THREE_WAY_VALVE = "three_way_valve"

nilan_ns = cg.esphome_ns.namespace('nilan')
 
CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(CONF_NILAN_ID): cv.use_id(Nilan),
    cv.Optional(CONF_IS_SUMMER): binary_sensor.BINARY_SENSOR_SCHEMA.extend({
        cv.Optional(CONF_DEVICE_CLASS, default=DEVICE_CLASS_EMPTY): binary_sensor.device_class
    }),
    cv.Optional(CONF_FILTER_OK): binary_sensor.BINARY_SENSOR_SCHEMA.extend({
        cv.Optional(CONF_DEVICE_CLASS, default=DEVICE_CLASS_EMPTY): binary_sensor.device_class
    }),
    cv.Optional(CONF_BYPASS_ON_OFF): binary_sensor.BINARY_SENSOR_SCHEMA.extend({
        cv.Optional(CONF_DEVICE_CLASS, default=DEVICE_CLASS_EMPTY): binary_sensor.device_class
    }),
    cv.Optional(CONF_DOOR_OPEN): binary_sensor.BINARY_SENSOR_SCHEMA.extend({
        cv.Optional(CONF_DEVICE_CLASS, default=DEVICE_CLASS_EMPTY): binary_sensor.device_class
    }),
    cv.Optional(CONF_AIRFLAP): binary_sensor.BINARY_SENSOR_SCHEMA.extend({
        cv.Optional(CONF_DEVICE_CLASS, default=DEVICE_CLASS_EMPTY): binary_sensor.device_class
    }),
    cv.Optional(CONF_SMOKEFLAP): binary_sensor.BINARY_SENSOR_SCHEMA.extend({
        cv.Optional(CONF_DEVICE_CLASS, default=DEVICE_CLASS_EMPTY): binary_sensor.device_class
    }),
    cv.Optional(CONF_AIRCIRC_PUMP): binary_sensor.BINARY_SENSOR_SCHEMA.extend({
        cv.Optional(CONF_DEVICE_CLASS, default=DEVICE_CLASS_EMPTY): binary_sensor.device_class
    }),
    cv.Optional(CONF_AIRHEAT_ALLOW): binary_sensor.BINARY_SENSOR_SCHEMA.extend({
        cv.Optional(CONF_DEVICE_CLASS, default=DEVICE_CLASS_EMPTY): binary_sensor.device_class
    }),
    cv.Optional(CONF_AIRHEAT_1): binary_sensor.BINARY_SENSOR_SCHEMA.extend({
        cv.Optional(CONF_DEVICE_CLASS, default=DEVICE_CLASS_EMPTY): binary_sensor.device_class
    }),
    cv.Optional(CONF_AIRHEAT_2): binary_sensor.BINARY_SENSOR_SCHEMA.extend({
        cv.Optional(CONF_DEVICE_CLASS, default=DEVICE_CLASS_EMPTY): binary_sensor.device_class
    }),
    cv.Optional(CONF_AIRHEAT_3): binary_sensor.BINARY_SENSOR_SCHEMA.extend({
        cv.Optional(CONF_DEVICE_CLASS, default=DEVICE_CLASS_EMPTY): binary_sensor.device_class
    }),
    cv.Optional(CONF_COMPRESSOR): binary_sensor.BINARY_SENSOR_SCHEMA.extend({
        cv.Optional(CONF_DEVICE_CLASS, default=DEVICE_CLASS_EMPTY): binary_sensor.device_class
    }),
    cv.Optional(CONF_COMPRESSOR_2): binary_sensor.BINARY_SENSOR_SCHEMA.extend({
        cv.Optional(CONF_DEVICE_CLASS, default=DEVICE_CLASS_EMPTY): binary_sensor.device_class
    }),
    cv.Optional(CONF_FOUR_WAY_COOL): binary_sensor.BINARY_SENSOR_SCHEMA.extend({
        cv.Optional(CONF_DEVICE_CLASS, default=DEVICE_CLASS_EMPTY): binary_sensor.device_class
    }),
    cv.Optional(CONF_HOTGAS_HEAT): binary_sensor.BINARY_SENSOR_SCHEMA.extend({
        cv.Optional(CONF_DEVICE_CLASS, default=DEVICE_CLASS_EMPTY): binary_sensor.device_class
    }),
    cv.Optional(CONF_HOTGAS_COOL): binary_sensor.BINARY_SENSOR_SCHEMA.extend({
        cv.Optional(CONF_DEVICE_CLASS, default=DEVICE_CLASS_EMPTY): binary_sensor.device_class
    }),
    cv.Optional(CONF_CONDENSER_OPEN): binary_sensor.BINARY_SENSOR_SCHEMA.extend({
        cv.Optional(CONF_DEVICE_CLASS, default=DEVICE_CLASS_EMPTY): binary_sensor.device_class
    }),
    cv.Optional(CONF_CONDENSER_CLOSE): binary_sensor.BINARY_SENSOR_SCHEMA.extend({
        cv.Optional(CONF_DEVICE_CLASS, default=DEVICE_CLASS_EMPTY): binary_sensor.device_class
    }),
    cv.Optional(CONF_WATER_HEAT): binary_sensor.BINARY_SENSOR_SCHEMA.extend({
        cv.Optional(CONF_DEVICE_CLASS, default=DEVICE_CLASS_EMPTY): binary_sensor.device_class
    }),
    cv.Optional(CONF_THREE_WAY_VALVE): binary_sensor.BINARY_SENSOR_SCHEMA.extend({
        cv.Optional(CONF_DEVICE_CLASS, default=DEVICE_CLASS_EMPTY): binary_sensor.device_class
    })
}).extend(cv.COMPONENT_SCHEMA)
 
def to_code(config):
    nilan = yield cg.get_variable(config[CONF_NILAN_ID])

    if CONF_IS_SUMMER in config:
        conf = config[CONF_IS_SUMMER]
        sens = yield binary_sensor.new_binary_sensor(conf)
        cg.add(nilan.set_is_summer_sensor(sens))
    if CONF_FILTER_OK in config:
        conf = config[CONF_FILTER_OK]        
        sens = yield binary_sensor.new_binary_sensor(conf)
        cg.add(nilan.set_filter_ok_sensor(sens))
    if CONF_BYPASS_ON_OFF in config:
        conf = config[CONF_BYPASS_ON_OFF]        
        sens = yield binary_sensor.new_binary_sensor(conf)
        cg.add(nilan.set_bypass_on_off_sensor(sens))
    if CONF_DOOR_OPEN in config:
        conf = config[CONF_DOOR_OPEN]        
        sens = yield binary_sensor.new_binary_sensor(conf)
        cg.add(nilan.set_door_open_sensor(sens))
    if CONF_AIRFLAP in config:
        conf = config[CONF_AIRFLAP]        
        sens = yield binary_sensor.new_binary_sensor(conf)
        cg.add(nilan.set_airflap_sensor(sens))
    if CONF_SMOKEFLAP in config:
        conf = config[CONF_SMOKEFLAP]        
        sens = yield binary_sensor.new_binary_sensor(conf)
        cg.add(nilan.set_smokeflap_sensor(sens))
    if CONF_AIRCIRC_PUMP in config:
        conf = config[CONF_AIRCIRC_PUMP]        
        sens = yield binary_sensor.new_binary_sensor(conf)
        cg.add(nilan.set_aircirc_pump_sensor(sens))
    if CONF_AIRHEAT_ALLOW in config:
        conf = config[CONF_AIRHEAT_ALLOW]        
        sens = yield binary_sensor.new_binary_sensor(conf)
        cg.add(nilan.set_airheat_allow_sensor(sens))
    if CONF_AIRHEAT_1 in config:
        conf = config[CONF_AIRHEAT_1]        
        sens = yield binary_sensor.new_binary_sensor(conf)
        cg.add(nilan.set_airheat_1_sensor(sens))
    if CONF_AIRHEAT_2 in config:
        conf = config[CONF_AIRHEAT_2]        
        sens = yield binary_sensor.new_binary_sensor(conf)
        cg.add(nilan.set_airheat_2_sensor(sens))
    if CONF_AIRHEAT_3 in config:
        conf = config[CONF_AIRHEAT_3]        
        sens = yield binary_sensor.new_binary_sensor(conf)
        cg.add(nilan.set_airheat_3_sensor(sens))
    if CONF_COMPRESSOR in config:
        conf = config[CONF_COMPRESSOR]        
        sens = yield binary_sensor.new_binary_sensor(conf)
        cg.add(nilan.set_compressor_sensor(sens))
    if CONF_COMPRESSOR_2 in config:
        conf = config[CONF_COMPRESSOR_2]        
        sens = yield binary_sensor.new_binary_sensor(conf)
        cg.add(nilan.set_compressor_2_sensor(sens))
    if CONF_FOUR_WAY_COOL in config:
        conf = config[CONF_FOUR_WAY_COOL]        
        sens = yield binary_sensor.new_binary_sensor(conf)
        cg.add(nilan.set_four_way_cool_sensor(sens))
    if CONF_HOTGAS_HEAT in config:
        conf = config[CONF_HOTGAS_HEAT]        
        sens = yield binary_sensor.new_binary_sensor(conf)
        cg.add(nilan.set_hotgas_heat_sensor(sens))
    if CONF_HOTGAS_COOL in config:
        conf = config[CONF_HOTGAS_COOL]        
        sens = yield binary_sensor.new_binary_sensor(conf)
        cg.add(nilan.set_hotgas_cool_sensor(sens))
    if CONF_CONDENSER_OPEN in config:
        conf = config[CONF_CONDENSER_OPEN]        
        sens = yield binary_sensor.new_binary_sensor(conf)
        cg.add(nilan.set_condenser_open_sensor(sens))
    if CONF_CONDENSER_CLOSE in config:
        conf = config[CONF_CONDENSER_CLOSE]        
        sens = yield binary_sensor.new_binary_sensor(conf)
        cg.add(nilan.set_condenser_close_sensor(sens))
    if CONF_WATER_HEAT in config:
        conf = config[CONF_WATER_HEAT]        
        sens = yield binary_sensor.new_binary_sensor(conf)
        cg.add(nilan.set_water_heat_sensor(sens))
    if CONF_THREE_WAY_VALVE in config:
        conf = config[CONF_THREE_WAY_VALVE]        
        sens = yield binary_sensor.new_binary_sensor(conf)
        cg.add(nilan.set_three_way_valve_sensor(sens))