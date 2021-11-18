import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID

nilan_ns = cg.esphome_ns.namespace('nilan')
Nilan = nilan_ns.class_('Nilan', cg.Component)

CONF_NILAN_ID = 'nilan_id'

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(Nilan),   
})

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config) 