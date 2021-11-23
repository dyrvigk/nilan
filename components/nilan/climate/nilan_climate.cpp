#include "nilan_climate.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace nilan {
static const char *TAG = "nilan.climate";

void NilanClimate::setup() {
  current_temp_sensor_->add_on_state_callback([this](float state) {
    // ESP_LOGD(TAG, "CURRENT TEMP SENSOR CALLBACK: %f", state);
    current_temperature = state;
    publish_state();
  });
  temp_setpoint_number_->add_on_state_callback([this](float state) {
    // ESP_LOGD(TAG, "TEMP SETPOINT SENSOR CALLBACK: %f", state);
    target_temperature = state;
    publish_state();
  });
  mode_select_->add_on_state_callback([this](std::string state) {
    // ESP_LOGD(TAG, "OPERATION MODE CALLBACK: %s", state.c_str());
    nilanmodetext_to_climatemode(state);
    publish_state();
  });
  fan_speed_number_->add_on_state_callback([this](float state) {
    // ESP_LOGD(TAG, "FAN SPEED SENSOR CALLBACK: %f", state);
    nilanfanspeed_to_fanmode(state);
    publish_state();
  });

  current_temperature = current_temp_sensor_->state;
  target_temperature  = temp_setpoint_number_->state;
  nilanmodetext_to_climatemode(mode_select_->state);
  nilanfanspeed_to_fanmode(fan_speed_number_->state); // Will update either fan_mode or custom_fan_mode
}

void NilanClimate::control(const climate::ClimateCall& call) {
  if (call.get_target_temperature().has_value())
  {
    this->target_temperature = *call.get_target_temperature();
    float target = target_temperature;
    ESP_LOGD(TAG, "Target temperature changed to: %f", target);
    temp_setpoint_number_->set(target);
  }

  if (call.get_mode().has_value())
  {
    auto new_mode = *call.get_mode();
    mode = new_mode;
    int operation_mode = climatemode_to_nilanoperationmode(new_mode);

    ESP_LOGD(TAG, "Operation mode changed to: %d", operation_mode);
    auto options = mode_select_->traits.get_options();

    if(operation_mode < options.size()) {
      mode_select_->set(options[operation_mode]);
    }
  }

  if (call.get_custom_fan_mode().has_value())
  {
    auto new_custom_fan_mode = *call.get_custom_fan_mode();
    custom_fan_mode = new_custom_fan_mode;
    fan_mode.reset();
    auto optional_nilan_fan_mode = parse_number<float>(new_custom_fan_mode.c_str(), new_custom_fan_mode.length());
    if(optional_nilan_fan_mode.has_value())
    {
      auto nilan_fan_mode = optional_nilan_fan_mode.value();
      ESP_LOGD(TAG, "Custom Fan mode set to: %i", nilan_fan_mode);
      fan_speed_number_->set(nilan_fan_mode);
    }
  }
  this->publish_state();
}

climate::ClimateTraits NilanClimate::traits() {
  auto traits = climate::ClimateTraits();

  traits.set_supported_custom_fan_modes({
    "1",
    "2",
    "3",
    "4"
  });

  traits.set_supported_fan_modes({ 
    climate::ClimateFanMode::CLIMATE_FAN_OFF 
  });

  traits.set_supported_modes({
    climate::ClimateMode::CLIMATE_MODE_COOL,
    climate::ClimateMode::CLIMATE_MODE_HEAT,
    climate::ClimateMode::CLIMATE_MODE_HEAT_COOL
   });

  traits.set_supports_current_temperature(true);
  traits.set_visual_temperature_step(1);
  traits.set_visual_min_temperature(5);
  traits.set_visual_max_temperature(30);

  return traits;
}

void NilanClimate::dump_config() {
  LOG_CLIMATE("", "Nilan Climate", this);
}

void NilanClimate::nilanfanspeed_to_fanmode(const int state)
{
  climate::ClimateFanMode return_value;

  switch (state) {
  case 1:
  case 2:
  case 3:
  case 4:
    this->custom_fan_mode = esphome::to_string(state);
    break;
  case 0:
  default: 
    this->fan_mode = climate::CLIMATE_FAN_OFF; 
    break;
  }
}

int NilanClimate::climatemode_to_nilanoperationmode(const climate::ClimateMode mode)
{
  int return_value;

  switch (mode) {
    case climate::CLIMATE_MODE_OFF: return_value = 0; break;
    case climate::CLIMATE_MODE_HEAT: return_value = 1; break;
    case climate::CLIMATE_MODE_COOL: return_value = 2; break;
    case climate::CLIMATE_MODE_HEAT_COOL: return_value = 3; break;
    default: return_value = 4; break;
  }

  return return_value;
}

void NilanClimate::nilanmodetext_to_climatemode(const std::string& nilan_mode)
{
  if (nilan_mode == "Off") {
    this->mode = climate::CLIMATE_MODE_OFF;
  }
  else if (nilan_mode == "Heat") {
    this->mode = climate::CLIMATE_MODE_HEAT;
  }
  else if (nilan_mode == "Cool") {
    this->mode = climate::CLIMATE_MODE_COOL;
  }
  else {
    this->mode = climate::CLIMATE_MODE_HEAT_COOL;
  }
}

} // namespace nilan
} // namespace esphome
