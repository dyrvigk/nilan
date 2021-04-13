import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from .. import Nilan, CONF_NILAN_ID
from esphome.const import (
    CONF_ID,
    CONF_TEMPERATURE,
    ICON_THERMOMETER,
    UNIT_CELSIUS,
    CONF_HUMIDITY,
    ICON_WATER_PERCENT,
    UNIT_PERCENT,
    ICON_EMPTY,
    UNIT_EMPTY,
    ICON_PERCENT,
    ICON_CHECK_CIRCLE_OUTLINE,
    ICON_GAUGE,
    CONF_BINARY_SENSOR,
    DEVICE_CLASS_TEMPERATURE,
    DEVICE_CLASS_HUMIDITY,
    DEVICE_CLASS_EMPTY,
)

DEPENDENCIES = ['nilan']

CONF_TEMP_T0 = "temp_t0"
CONF_TEMP_T3 = "temp_t3"
CONF_TEMP_T4 = "temp_t4"
CONF_TEMP_T7 = "temp_t7"
CONF_TEMP_T8 = "temp_t8"
CONF_TEMP_T15 = "temp_t15"
CONF_MEASURED_HUMIDITY = "measured_humidity"
CONF_ACTIVE_ALARMS = "active_alarms"
CONF_COOL_TARGET_TEMP = "cool_target_temp"
CONF_TEMP_MIN_SUMMER = "min_summer_temp"
CONF_TEMP_MIN_WINTER = "min_winter_temp"
CONF_TEMP_MAX_SUMMER = "max_summer_temp"
CONF_TEMP_MAX_WINTER = "max_winter_temp"
CONF_IS_SUMMER = "is_summer"
CONF_HEAT_EXCHANGE_EFFICIENCY = "heat_exchange_efficiency"

#CONF_HUMIDITY_CALCULATED_SETPOINT = "humidity_calculated_setpoint"
#CONF_ALARM_BIT = "alarm_bit"
CONF_INLET_FAN = "inlet_fan"
CONF_EXTRACT_FAN = "extract_fan"
CONF_BYPASS = "bypass"
CONF_WATERVALVE = "watervalve"
CONF_HUMIDITY_FAN_CONTROL = "humidity_fan_control"
CONF_BYPASS_ON_OFF = "bypass_on_off"
#CONF_TARGET_TEMP = "target_temp"
CONF_SPEED_MODE = "speed_mode"
CONF_HEAT = "heat"
CONF_TIMER = "timer"

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(CONF_NILAN_ID): cv.use_id(Nilan),
    cv.Required(CONF_TEMP_T0): sensor.sensor_schema(UNIT_CELSIUS, ICON_THERMOMETER, 1, DEVICE_CLASS_TEMPERATURE),
    cv.Optional(CONF_TEMP_T3): sensor.sensor_schema(UNIT_CELSIUS, ICON_THERMOMETER, 1, DEVICE_CLASS_TEMPERATURE),
    cv.Optional(CONF_TEMP_T4): sensor.sensor_schema(UNIT_CELSIUS, ICON_THERMOMETER, 1, DEVICE_CLASS_TEMPERATURE),
    cv.Optional(CONF_TEMP_T7): sensor.sensor_schema(UNIT_CELSIUS, ICON_THERMOMETER, 1, DEVICE_CLASS_TEMPERATURE),
    cv.Optional(CONF_TEMP_T8): sensor.sensor_schema(UNIT_CELSIUS, ICON_THERMOMETER, 1, DEVICE_CLASS_TEMPERATURE),
    cv.Optional(CONF_TEMP_T15): sensor.sensor_schema(UNIT_CELSIUS, ICON_THERMOMETER, 1, DEVICE_CLASS_TEMPERATURE),
    cv.Optional(CONF_MEASURED_HUMIDITY): sensor.sensor_schema(UNIT_PERCENT, ICON_WATER_PERCENT, 1, DEVICE_CLASS_HUMIDITY),
    cv.Optional(CONF_ACTIVE_ALARMS): sensor.sensor_schema(UNIT_EMPTY, ICON_EMPTY, 0, DEVICE_CLASS_EMPTY),
    cv.Optional(CONF_COOL_TARGET_TEMP): sensor.sensor_schema(UNIT_CELSIUS, ICON_THERMOMETER, 1, DEVICE_CLASS_TEMPERATURE),
    cv.Optional(CONF_TEMP_MIN_SUMMER): sensor.sensor_schema(UNIT_CELSIUS, ICON_THERMOMETER, 1, DEVICE_CLASS_TEMPERATURE),
    cv.Optional(CONF_TEMP_MAX_SUMMER): sensor.sensor_schema(UNIT_CELSIUS, ICON_THERMOMETER, 1, DEVICE_CLASS_TEMPERATURE),
    cv.Optional(CONF_TEMP_MIN_WINTER): sensor.sensor_schema(UNIT_CELSIUS, ICON_THERMOMETER, 1, DEVICE_CLASS_TEMPERATURE),
    cv.Optional(CONF_TEMP_MAX_WINTER): sensor.sensor_schema(UNIT_CELSIUS, ICON_THERMOMETER, 1, DEVICE_CLASS_TEMPERATURE),
    cv.Optional(CONF_IS_SUMMER): sensor.sensor_schema(UNIT_EMPTY, ICON_CHECK_CIRCLE_OUTLINE, 1, DEVICE_CLASS_EMPTY),
    cv.Optional(CONF_HEAT_EXCHANGE_EFFICIENCY): sensor.sensor_schema(UNIT_PERCENT, ICON_PERCENT, 1, DEVICE_CLASS_EMPTY),
    
#    cv.Optional(CONF_HUMIDITY_CALCULATED_SETPOINT): sensor.sensor_schema(UNIT_PERCENT, ICON_WATER_PERCENT, 1, DEVICE_CLASS_HUMIDITY),
#    cv.Optional(CONF_ALARM_BIT): sensor.sensor_schema(UNIT_EMPTY, ICON_EMPTY, 1, DEVICE_CLASS_EMPTY),
    cv.Optional(CONF_INLET_FAN): sensor.sensor_schema(UNIT_PERCENT, ICON_PERCENT, 1, DEVICE_CLASS_EMPTY),
    cv.Optional(CONF_EXTRACT_FAN): sensor.sensor_schema(UNIT_PERCENT, ICON_PERCENT, 1, DEVICE_CLASS_EMPTY),
    cv.Optional(CONF_BYPASS): sensor.sensor_schema(UNIT_PERCENT, ICON_PERCENT, 1, DEVICE_CLASS_EMPTY),
    cv.Optional(CONF_WATERVALVE): sensor.sensor_schema(UNIT_PERCENT, ICON_PERCENT, 1, DEVICE_CLASS_EMPTY),
    cv.Optional(CONF_HUMIDITY_FAN_CONTROL): sensor.sensor_schema(UNIT_PERCENT, ICON_PERCENT, 1, DEVICE_CLASS_EMPTY),
    cv.Optional(CONF_BYPASS_ON_OFF): sensor.sensor_schema(UNIT_EMPTY, ICON_CHECK_CIRCLE_OUTLINE, 1, DEVICE_CLASS_EMPTY),
#    cv.Required(CONF_TARGET_TEMP): sensor.sensor_schema(UNIT_CELSIUS, ICON_THERMOMETER, 1, DEVICE_CLASS_TEMPERATURE),
    cv.Required(CONF_SPEED_MODE): sensor.sensor_schema(UNIT_EMPTY, ICON_GAUGE, 1, DEVICE_CLASS_EMPTY),
    cv.Optional(CONF_HEAT): sensor.sensor_schema(UNIT_CELSIUS, ICON_THERMOMETER, 1, DEVICE_CLASS_EMPTY),
    cv.Optional(CONF_TIMER): sensor.sensor_schema(UNIT_EMPTY, ICON_GAUGE, 1, DEVICE_CLASS_EMPTY)
}).extend(cv.polling_component_schema('60s'))


def to_code(config):
    nilan = yield cg.get_variable(config[CONF_NILAN_ID])

    if CONF_TEMP_T0 in config:
        conf = config[CONF_TEMP_T0]
        sens = yield sensor.new_sensor(conf)
        cg.add(nilan.set_temp_t0_sensor(sens))
    if CONF_TEMP_T3 in config:
        conf = config[CONF_TEMP_T3]
        sens = yield sensor.new_sensor(conf)
        cg.add(nilan.set_temp_t3_sensor(sens))
    if CONF_TEMP_T4 in config:
        conf = config[CONF_TEMP_T4]
        sens = yield sensor.new_sensor(conf)
        cg.add(nilan.set_temp_t4_sensor(sens))
    if CONF_TEMP_T7 in config:
        conf = config[CONF_TEMP_T7]
        sens = yield sensor.new_sensor(conf)
        cg.add(nilan.set_temp_t7_sensor(sens))
    if CONF_TEMP_T8 in config:
        conf = config[CONF_TEMP_T8]
        sens = yield sensor.new_sensor(conf)
        cg.add(nilan.set_temp_t8_sensor(sens))
    if CONF_TEMP_T15 in config:
        conf = config[CONF_TEMP_T15]
        sens = yield sensor.new_sensor(conf)
        cg.add(nilan.set_temp_t15_sensor(sens))
    if CONF_MEASURED_HUMIDITY in config:
        conf = config[CONF_MEASURED_HUMIDITY]
        sens = yield sensor.new_sensor(conf)
        cg.add(nilan.set_measured_humidity_sensor(sens))
    if CONF_ACTIVE_ALARMS in config:
        conf = config[CONF_ACTIVE_ALARMS]
        sens = yield sensor.new_sensor(conf)
        cg.add(nilan.set_active_alarms_sensor(sens))
    if CONF_COOL_TARGET_TEMP in config:
        conf = config[CONF_COOL_TARGET_TEMP]
        sens = yield sensor.new_sensor(conf)
        cg.add(nilan.set_cool_target_temp_sensor(sens))
    if CONF_TEMP_MIN_SUMMER in config:
        conf = config[CONF_TEMP_MIN_SUMMER]
        sens = yield sensor.new_sensor(conf)
        cg.add(nilan.set_min_summer_temp_sensor(sens))
    if CONF_TEMP_MAX_SUMMER in config:
        conf = config[CONF_TEMP_MAX_SUMMER]
        sens = yield sensor.new_sensor(conf)
        cg.add(nilan.set_max_summer_temp_sensor(sens))
    if CONF_TEMP_MIN_WINTER in config:
        conf = config[CONF_TEMP_MIN_WINTER]
        sens = yield sensor.new_sensor(conf)
        cg.add(nilan.set_min_winter_temp_sensor(sens))
    if CONF_TEMP_MAX_WINTER in config:
        conf = config[CONF_TEMP_MAX_WINTER]
        sens = yield sensor.new_sensor(conf)
        cg.add(nilan.set_max_winter_temp_sensor(sens))
    if CONF_IS_SUMMER in config:
        conf = config[CONF_IS_SUMMER]
        sens = yield sensor.new_sensor(conf)
        cg.add(nilan.set_is_summer_sensor(sens))
    if CONF_HEAT_EXCHANGE_EFFICIENCY in config:
        conf = config[CONF_HEAT_EXCHANGE_EFFICIENCY]
        sens = yield sensor.new_sensor(conf)
        cg.add(nilan.set_heat_exchange_efficiency_sensor(sens))
        
#    if CONF_HUMIDITY_CALCULATED_SETPOINT in config:
#        conf = config[CONF_HUMIDITY_CALCULATED_SETPOINT]
#        sens = yield sensor.new_sensor(conf)
#        cg.add(nilan.set_humidity_calculated_setpoint_sensor(sens))
#    if CONF_ALARM_BIT in config:
#        conf = config[CONF_ALARM_BIT]
#        sens = yield sensor.new_sensor(conf)
#        cg.add(nilan.set_alarm_bit_sensor(sens))
    if CONF_INLET_FAN in config:
        conf = config[CONF_INLET_FAN]
        sens = yield sensor.new_sensor(conf)
        cg.add(nilan.set_inlet_fan_sensor(sens))
    if CONF_EXTRACT_FAN in config:
        conf = config[CONF_EXTRACT_FAN]
        sens = yield sensor.new_sensor(conf)
        cg.add(nilan.set_extract_fan_sensor(sens))
    if CONF_BYPASS in config:
        conf = config[CONF_BYPASS]
        sens = yield sensor.new_sensor(conf)
        cg.add(nilan.set_bypass_sensor(sens))
    if CONF_WATERVALVE in config:
        conf = config[CONF_WATERVALVE]
        sens = yield sensor.new_sensor(conf)
        cg.add(nilan.set_watervalve_sensor(sens))
    if CONF_HUMIDITY_FAN_CONTROL in config:
        conf = config[CONF_HUMIDITY_FAN_CONTROL]
        sens = yield sensor.new_sensor(conf)
        cg.add(nilan.set_humidity_fan_control_sensor(sens))
    if CONF_BYPASS_ON_OFF in config:
        conf = config[CONF_BYPASS_ON_OFF]
        sens = yield sensor.new_sensor(conf)
        cg.add(nilan.set_bypass_on_off_sensor(sens))
#    if CONF_TARGET_TEMP in config:
#        conf = config[CONF_TARGET_TEMP]
#        sens = yield sensor.new_sensor(conf)
#        cg.add(nilan.set_target_temp_sensor(sens))
    if CONF_SPEED_MODE in config:
        conf = config[CONF_SPEED_MODE]
        sens = yield sensor.new_sensor(conf)
        cg.add(nilan.set_speed_mode_sensor(sens))
    if CONF_HEAT in config:
        conf = config[CONF_HEAT]
        sens = yield sensor.new_sensor(conf)
        cg.add(nilan.set_heat_sensor(sens))
    if CONF_TIMER in config:
        conf = config[CONF_TIMER]
        sens = yield sensor.new_sensor(conf)
        cg.add(nilan.set_timer_sensor(sens))          
        
