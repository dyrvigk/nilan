#pragma once

#include "../nilan.h"
#include "esphome/core/component.h"
#include "esphome/components/climate/climate.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"

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
  void set_on_off_sensor(binary_sensor::BinarySensor* sensor) { this->on_off_sensor_ = sensor; }

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
  /// The sensor used for getting the machine on/off mode
  binary_sensor::BinarySensor* on_off_sensor_{ nullptr };

 private:
   climate::ClimateFanMode nilanfanspeed_to_fanmode(int state)
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

   int climatemode_to_nilanoperationmode(climate::ClimateMode mode)
   {
     int return_value;
     switch (mode) {
     case climate::CLIMATE_MODE_OFF: return_value = 0; break;
     case climate::CLIMATE_MODE_HEAT: return_value = 1; break;
     case climate::CLIMATE_MODE_COOL: return_value = 2; break;
     case climate::CLIMATE_MODE_AUTO: return_value = 3; break;
     default: return_value = 4; break;
     }

     return return_value;
   }


};

}  // namespace nilan
}  // namespace esphome
