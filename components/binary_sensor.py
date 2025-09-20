import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
from esphome.const import CONF_ID

# Configuration constants
CONF_MODBUS_TCP_ID = "modbus_tcp_id"

# Import from main component
from . import modbus_tcp_ns, ModbusTCPManager

ModbusTCPConnectionSensor = modbus_tcp_ns.class_("ModbusTCPConnectionSensor", cg.PollingComponent, binary_sensor.BinarySensor)

# Dependencies
DEPENDENCIES = ["network"]

# Configuration schema
CONFIG_SCHEMA = binary_sensor.binary_sensor_schema(ModbusTCPConnectionSensor).extend({
    cv.GenerateID(CONF_MODBUS_TCP_ID): cv.use_id(ModbusTCPManager),
}).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    parent = await cg.get_variable(config[CONF_MODBUS_TCP_ID])
    
    var = cg.new_Pvariable(config[CONF_ID], parent)
    
    await cg.register_component(var, config)
    await binary_sensor.register_binary_sensor(var, config)
