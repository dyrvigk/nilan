#include "nilan_climate.h"
#include "esphome/core/log.h"

namespace esphome {
namespace nilan {

static const char *TAG = "nilan.climate";

void NilanClimate::setup() {
  current_temp_sensor_->add_on_state_callback([this](float state) {
    ESP_LOGD(TAG, "CURRENT TEMP SENSOR CALLBACK: %f", state);
    current_temperature = state;
    publish_state();
  });
  temp_setpoint_sensor_->add_on_state_callback([this](float state) {
    ESP_LOGD(TAG, "TEMP SETPOINT SENSOR CALLBACK: %f", state);
    target_temperature = state;
    publish_state();
  });
  mode_sensor_->add_on_state_callback([this](std::string state) {
    ESP_LOGD(TAG, "OPERATION MODE CALLBACK: %s", state.c_str());
    mode = nilanmodetext_to_climatemode(state);
    publish_state();
  });
  fan_speed_sensor_->add_on_state_callback([this](float state) {
    ESP_LOGD(TAG, "FAN SPEED SENSOR CALLBACK: %f", state);
    fan_mode = nilanfanspeed_to_fanmode(state);
    publish_state();
  });

  /*nilan_->add_target_temp_callback([this](float state) {
    ESP_LOGD(TAG, "TARGET TEMP CHANGE CALLBACK: %f", state);
    target_temperature = state;
    publish_state();
  });
  nilan_->add_operation_mode_callback([this](float state) {
    ESP_LOGD(TAG, "OPERATION MODE CHANGE CALLBACK: %f", state);
    mode = (esphome::climate::ClimateMode)((int)state);
    publish_state();
  });
  nilan_->add_fan_speed_callback([this](int state) {
    ESP_LOGD(TAG, "FAN SPEED CHANGE CALLBACK: %d", state);
    fan_mode = nilanfanspeed_to_fanmode(state);
    publish_state();
  });*/

  current_temperature = current_temp_sensor_->state;
  target_temperature = temp_setpoint_sensor_->state;
  fan_mode = nilanfanspeed_to_fanmode(fan_speed_sensor_->state);
  mode = nilanmodetext_to_climatemode(mode_sensor_->state);
}

void NilanClimate::control(const climate::ClimateCall &call) {
  if (call.get_target_temperature().has_value())
  {
    this->target_temperature = *call.get_target_temperature();
    float target = target_temperature;
    ESP_LOGD(TAG, "Target temperature changed to: %f", target);
    nilan_->writeTargetTemperature(target);
  }

  if (call.get_mode().has_value())
  {
    auto new_mode = *call.get_mode();
    mode = new_mode;
    int operation_mode = climatemode_to_nilanoperationmode(new_mode);

    ESP_LOGD(TAG, "Operation mode changed to: %d", operation_mode);
    if (operation_mode > 0) {
      nilan_->writeRunset(1);
      nilan_->writeOperationMode(operation_mode);
    }
    else {
      nilan_->writeRunset(0);
    }
  }
  
  if (call.get_fan_mode().has_value())
  {
    int nilan_fan_mode;
    auto new_fan_mode = *call.get_fan_mode();
    fan_mode = new_fan_mode;
    switch (new_fan_mode) {
      case climate::CLIMATE_FAN_LOW: nilan_fan_mode = 2; break;
      case climate::CLIMATE_FAN_MEDIUM: nilan_fan_mode = 3; break;
      case climate::CLIMATE_FAN_HIGH: nilan_fan_mode = 4; break;
      case climate::CLIMATE_FAN_OFF: nilan_fan_mode = 0; break;
      default: nilan_fan_mode = 2; break;
    }
    ESP_LOGD(TAG, "Fan mode set to: %i", nilan_fan_mode);
     
    nilan_->writeFanMode(nilan_fan_mode);
  }
  this->publish_state();
}

climate::ClimateTraits NilanClimate::traits() {
  auto traits = climate::ClimateTraits();

  //traits.set_supports_auto_mode(true);
  traits.set_supports_heat_cool_mode(true);
  traits.set_supports_cool_mode(true);
  traits.set_supports_heat_mode(true);
  
  traits.set_supports_fan_mode_off(true);
  traits.set_supports_fan_mode_low(true);
  traits.set_supports_fan_mode_medium(true);
  traits.set_supports_fan_mode_high(true);
  
  traits.set_supports_current_temperature(true);
  traits.set_visual_temperature_step(1);
  traits.set_visual_min_temperature(5);
  traits.set_visual_max_temperature(30);
  
  return traits;
}

void NilanClimate::dump_config() {
  LOG_CLIMATE("", "Nilan Climate", this);
}

}  // namespace nilan
}  // namespace esphome
