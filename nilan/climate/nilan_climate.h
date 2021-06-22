#pragma once

#include "../nilan.h"
#include "esphome/core/component.h"
#include "esphome/components/climate/climate.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"

namespace esphome {
namespace nilan {

class NilanClimate : public climate::Climate, public Component {
 public:
  NilanClimate(Nilan *nilan) : nilan_(nilan) {}
  void setup() override;
  void dump_config() override;

  void set_current_temp_sensor(sensor::Sensor *sensor) { this->current_temp_sensor_ = sensor; }
  void set_temp_setpoint_sensor(sensor::Sensor *sensor) { this->temp_setpoint_sensor_ = sensor; }
  void set_fan_speed_sensor(sensor::Sensor* sensor) { this->fan_speed_sensor_ = sensor; }
  void set_mode_sensor(text_sensor::TextSensor* sensor) { this->mode_sensor_ = sensor; }

 protected:
  Nilan *nilan_;
  /// Override control to change settings of the climate device.
  void control(const climate::ClimateCall &call) override;
  /// Return the traits of this controller.
  climate::ClimateTraits traits() override;

  /// The sensor used for getting the current temperature
  sensor::Sensor *current_temp_sensor_{nullptr};
  /// The sensor used for getting the temperature setpoint
  sensor::Sensor *temp_setpoint_sensor_{nullptr};
  /// The sensor used for getting fan speed
  sensor::Sensor* fan_speed_sensor_{ nullptr };
  /// The text sensor used for getting the operation mode
  text_sensor::TextSensor* mode_sensor_{ nullptr };

 private:
   climate::ClimateFanMode nilanfanspeed_to_fanmode(const int state)
   {
     climate::ClimateFanMode return_value;
     switch (state) {
       case 0: return_value = climate::CLIMATE_FAN_OFF; break;
       case 2: return_value = climate::CLIMATE_FAN_LOW; break;
       case 3: return_value = climate::CLIMATE_FAN_MEDIUM; break;
       case 4: return_value = climate::CLIMATE_FAN_HIGH; break;
       default: return_value = climate::CLIMATE_FAN_ON; break;
     }

     return return_value;
   }

   int climatemode_to_nilanoperationmode(const climate::ClimateMode mode)
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

   climate::ClimateMode nilanmodetext_to_climatemode(const std::string & mode)
   {
     climate::ClimateMode return_value;

     if (mode == "Off") {
       return_value = climate::CLIMATE_MODE_OFF;
     }
     else if (mode == "Heat") {
       return_value = climate::CLIMATE_MODE_HEAT;
     }
     else if (mode == "Cool") {
       return_value = climate::CLIMATE_MODE_COOL;
     }
     else {
       return_value = climate::CLIMATE_MODE_HEAT_COOL;
     }

     return return_value;
   }
};

}  // namespace nilan
}  // namespace esphome
