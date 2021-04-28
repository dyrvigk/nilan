#pragma once

#include "esphome/core/component.h"
#include "esphome/core/helpers.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
//#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/modbus/modbus.h"

namespace esphome {
namespace nilan {

class Nilan : public PollingComponent, public modbus::ModbusDevice {
 public: 
  /*Nilan() {
    operation_mode_sensor_ = new text_sensor::TextSensor("Operation mode");
    this->register_text_sensor(operation_mode_sensor_);
  }*/
  void set_temp_t0_sensor(sensor::Sensor * temp_t0_sensor) { temp_t0_sensor_ = temp_t0_sensor; }
  void set_temp_t3_sensor(sensor::Sensor * temp_t3_sensor) { temp_t3_sensor_ = temp_t3_sensor; }
  void set_temp_t4_sensor(sensor::Sensor * temp_t4_sensor) { temp_t4_sensor_ = temp_t4_sensor; }
  void set_temp_t7_sensor(sensor::Sensor * temp_t7_sensor) { temp_t7_sensor_ = temp_t7_sensor; }
  void set_temp_t8_sensor(sensor::Sensor * temp_t8_sensor) { temp_t8_sensor_ = temp_t8_sensor; }
  void set_temp_t15_sensor(sensor::Sensor * temp_t15_sensor) { temp_t15_sensor_ = temp_t15_sensor; }
  void set_measured_humidity_sensor(sensor::Sensor *measured_humidity_sensor) { measured_humidity_sensor_ = measured_humidity_sensor; }
  void set_active_alarms_sensor(sensor::Sensor *active_alarms_sensor) { active_alarms_sensor_ = active_alarms_sensor; }
  void set_cool_target_temp_sensor(sensor::Sensor *cool_target_temp_sensor) { cool_target_temp_sensor_ = cool_target_temp_sensor; }
  void set_min_winter_temp_sensor(sensor::Sensor *min_winter_temp_sensor) { min_winter_temp_sensor_ = min_winter_temp_sensor; }
  void set_max_winter_temp_sensor(sensor::Sensor *max_winter_temp_sensor) { max_winter_temp_sensor_ = max_winter_temp_sensor; }
  void set_min_summer_temp_sensor(sensor::Sensor *min_summer_temp_sensor) { min_summer_temp_sensor_ = min_summer_temp_sensor; }
  void set_max_summer_temp_sensor(sensor::Sensor *max_summer_temp_sensor) { max_summer_temp_sensor_ = max_summer_temp_sensor; }
  void set_heat_exchange_efficiency_sensor(sensor::Sensor *heat_exchange_efficiency_sensor) { heat_exchange_efficiency_sensor_ = heat_exchange_efficiency_sensor; }
  void set_operation_mode_sensor(sensor::Sensor *operation_mode_sensor) { operation_mode_sensor_ = operation_mode_sensor; }
  void set_control_state_sensor(sensor::Sensor *control_state_sensor) { control_state_sensor_ = control_state_sensor; }

  void set_is_summer_sensor(binary_sensor::BinarySensor *is_summer_sensor) { is_summer_sensor_ = is_summer_sensor; }
  void set_filter_ok_sensor(binary_sensor::BinarySensor *filter_ok_sensor) { filter_ok_sensor_ = filter_ok_sensor; }
  void set_door_open_sensor(binary_sensor::BinarySensor *door_open_sensor) { door_open_sensor_ = door_open_sensor; }
  void set_bypass_on_off_sensor(binary_sensor::BinarySensor *bypass_on_off_sensor) { bypass_on_off_sensor_ = bypass_on_off_sensor; }
  void set_on_off_state_sensor(binary_sensor::BinarySensor *on_off_state_sensor) { on_off_state_sensor_ = on_off_state_sensor; }

  void set_inlet_fan_sensor(sensor::Sensor *inlet_fan_sensor) { inlet_fan_sensor_ = inlet_fan_sensor; }
  void set_extract_fan_sensor(sensor::Sensor *extract_fan_sensor) { extract_fan_sensor_ = extract_fan_sensor; }
  void set_bypass_sensor(sensor::Sensor *bypass_sensor) { bypass_sensor_ = bypass_sensor; }
  void set_watervalve_sensor(sensor::Sensor *watervalve_sensor) { watervalve_sensor_ = watervalve_sensor; }
  void set_humidity_fan_control_sensor(sensor::Sensor *humidity_fan_control_sensor) { humidity_fan_control_sensor_ = humidity_fan_control_sensor; }
//  void set_target_temp_sensor(sensor::Sensor *target_temp_sensor) { target_temp_sensor_ = target_temp_sensor; }
  void set_speed_mode_sensor(sensor::Sensor *speed_mode_sensor) { speed_mode_sensor_ = speed_mode_sensor; }
  void set_heat_sensor(sensor::Sensor *heat_sensor) { heat_sensor_ = heat_sensor; }
  void set_timer_sensor(sensor::Sensor *timer_sensor) { timer_sensor_ = timer_sensor; }

  void add_target_temp_callback(std::function<void(float)> &&callback);
  void add_fan_speed_callback(std::function<void(int)> &&callback);
  
  void loop() override;
  void update() override;

  void on_modbus_data(const std::vector<uint8_t> &data) override;
  
  void handleTemperatureData(const std::vector<uint8_t> &data);
  void handleAlarmData(const std::vector<uint8_t> &data);
  void handleSpecificAlarms(const std::vector<uint8_t> &data);
  void handleAirtempHoldingData(const std::vector<uint8_t> &data);
  void handleAirtempInputData(const std::vector<uint8_t> &data);
  void handleControlStateInputData(const std::vector<uint8_t> &data);
  void handleControlStateHoldingData(const std::vector<uint8_t>& data);
  void handleVersionInfoData(const std::vector<uint8_t> &data);
  
  void publishState(sensor::Sensor * sensor, float value) {
    if(sensor && (sensor->state != value || ignore_previous_state_))
      sensor->publish_state(value);
  }   
  void publishState(binary_sensor::BinarySensor * sensor, bool value) {
    if(sensor && (sensor->state != value || ignore_previous_state_))
      sensor->publish_state(value);
  }

  void writeTargetTemperature(float new_target_temp);
  void writeFanMode(int new_fan_speed);
  
  void dump_config() override;

 protected:
  enum ReadState { 
    idle = 0,
    temperatures = 1,
    alarms = 2,
    specific_alarms = 3,
    airtemp_holding = 4,
    airtemp_input = 5,
    control_state_input = 6,
    control_state_holding = 7,
    version_info = 8
  };
  
  ReadState read_state_{idle};
  uint8_t CMD_FUNCTION_REG{0x04};
  bool waiting_{false};
  long last_send_{0};
  
  sensor::Sensor *temp_t0_sensor_;
  sensor::Sensor *temp_t3_sensor_;
  sensor::Sensor *temp_t4_sensor_;
  sensor::Sensor *temp_t7_sensor_;
  sensor::Sensor *temp_t8_sensor_;
  sensor::Sensor *temp_t15_sensor_;
  sensor::Sensor *measured_humidity_sensor_;
  sensor::Sensor *active_alarms_sensor_;
  sensor::Sensor *cool_target_temp_sensor_;
  sensor::Sensor *min_summer_temp_sensor_;
  sensor::Sensor *max_summer_temp_sensor_;
  sensor::Sensor *min_winter_temp_sensor_;
  sensor::Sensor *max_winter_temp_sensor_;
  sensor::Sensor *heat_exchange_efficiency_sensor_;
  sensor::Sensor *control_state_sensor_;
  sensor::Sensor *operation_mode_sensor_;
  binary_sensor::BinarySensor *on_off_state_sensor_;
  binary_sensor::BinarySensor *is_summer_sensor_;
  binary_sensor::BinarySensor *filter_ok_sensor_;
  binary_sensor::BinarySensor *door_open_sensor_;
  binary_sensor::BinarySensor *bypass_on_off_sensor_;
  //text_sensor::TextSensor *version_info_sensor_;
  
  sensor::Sensor *inlet_fan_sensor_;
  sensor::Sensor *extract_fan_sensor_;
  sensor::Sensor *bypass_sensor_;
  sensor::Sensor *watervalve_sensor_;
  sensor::Sensor *humidity_fan_control_sensor_;
  //sensor::Sensor *target_temp_sensor_;
  sensor::Sensor *speed_mode_sensor_;
  sensor::Sensor *heat_sensor_;
  sensor::Sensor *timer_sensor_;

  CallbackManager<void(float)> target_temp_callback_;
  CallbackManager<void(int)> fan_speed_callback_;
  
 private:
   uint16_t get_16bit(const std::vector<uint8_t> &data, size_t i) { return (uint16_t(data[i]) << 8) | uint16_t(data[i + 1]); };
   float convertToTemperature(uint16_t rawValue) { return static_cast<int16_t>(rawValue) / 100.0; };

   bool ignore_previous_state_ = true;
};

}  // namespace nilan
}  // namespace esphome
