#pragma once

#include "esphome/core/component.h"
#include "esphome/core/helpers.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/select/select.h"
#include "esphome/components/modbus_controller/modbus_controller.h"
#include "nilan_types.h"

namespace esphome {
namespace nilan {

class Nilan : public modbus_controller::ModbusController {
public:
  void set_is_summer_sensor(binary_sensor::BinarySensor *is_summer_sensor) {
    is_summer_sensor_ = is_summer_sensor;
  }

  void set_filter_ok_sensor(binary_sensor::BinarySensor *filter_ok_sensor) {
    filter_ok_sensor_ = filter_ok_sensor;
  }

  void set_door_open_sensor(binary_sensor::BinarySensor *door_open_sensor) {
    door_open_sensor_ = door_open_sensor;
  }

  void set_bypass_on_off_sensor(binary_sensor::BinarySensor *bypass_on_off_sensor) {
    bypass_on_off_sensor_ = bypass_on_off_sensor;
  }

  void set_on_off_state_sensor(binary_sensor::BinarySensor *on_off_state_sensor) {
    on_off_state_sensor_ = on_off_state_sensor;
  }

  void set_airflap_sensor(binary_sensor::BinarySensor *airflap_sensor) {
    airflap_sensor_ = airflap_sensor;
  }

  void set_smokeflap_sensor(binary_sensor::BinarySensor *smokeflap_sensor) {
    smokeflap_sensor_ = smokeflap_sensor;
  }

  void set_aircirc_pump_sensor(binary_sensor::BinarySensor *aircirc_pump_sensor) {
    aircirc_pump_sensor_ = aircirc_pump_sensor;
  }

  void set_airheat_allow_sensor(binary_sensor::BinarySensor *airheat_allow_sensor) {
    airheat_allow_sensor_ = airheat_allow_sensor;
  }

  void set_airheat_1_sensor(binary_sensor::BinarySensor *airheat_1_sensor) {
    airheat_1_sensor_ = airheat_1_sensor;
  }

  void set_airheat_2_sensor(binary_sensor::BinarySensor *airheat_2_sensor) {
    airheat_2_sensor_ = airheat_2_sensor;
  }

  void set_airheat_3_sensor(binary_sensor::BinarySensor *airheat_3_sensor) {
    airheat_3_sensor_ = airheat_3_sensor;
  }

  void set_compressor_sensor(binary_sensor::BinarySensor *compressor_sensor) {
    compressor_sensor_ = compressor_sensor;
  }

  void set_compressor_2_sensor(binary_sensor::BinarySensor *compressor_2_sensor) {
    compressor_2_sensor_ = compressor_2_sensor;
  }

  void set_four_way_cool_sensor(binary_sensor::BinarySensor *four_way_cool_sensor) {
    four_way_cool_sensor_ = four_way_cool_sensor;
  }

  void set_hotgas_heat_sensor(binary_sensor::BinarySensor *hotgas_heat_sensor) {
    hotgas_heat_sensor_ = hotgas_heat_sensor;
  }

  void set_hotgas_cool_sensor(binary_sensor::BinarySensor *hotgas_cool_sensor) {
    hotgas_cool_sensor_ = hotgas_cool_sensor;
  }

  void set_condenser_open_sensor(binary_sensor::BinarySensor *condenser_open_sensor) {
    condenser_open_sensor_ = condenser_open_sensor;
  }

  void set_condenser_close_sensor(binary_sensor::BinarySensor *condenser_close_sensor) {
    condenser_close_sensor_ = condenser_close_sensor;
  }

  void set_water_heat_sensor(binary_sensor::BinarySensor *water_heat_sensor) {
    water_heat_sensor_ = water_heat_sensor;
  }

  void set_three_way_valve_sensor(binary_sensor::BinarySensor *three_way_valve_sensor) {
    three_way_valve_sensor_ = three_way_valve_sensor;
  }

  void set_operation_mode_sensor(text_sensor::TextSensor *operation_mode_sensor) {
    operation_mode_sensor_ = operation_mode_sensor;
  }

  void set_control_state_sensor(text_sensor::TextSensor *control_state_sensor) {
    control_state_sensor_ = control_state_sensor;
  }

  void set_version_info_sensor(text_sensor::TextSensor *version_info_sensor) {
    version_info_sensor_ = version_info_sensor;
  }

  void set_display_line1_sensor(text_sensor::TextSensor *display_line1_sensor) {
    display_line1_sensor_ = display_line1_sensor;
  }

  void set_display_line2_sensor(text_sensor::TextSensor *display_line2_sensor) {
    display_line2_sensor_ = display_line2_sensor;
  }

  void set_user_function_select(select::Select *user_function_select) {
    user_function_select_ = user_function_select;
  }

  void set_user_function_actual_sensor(text_sensor::TextSensor *user_function_actual_sensor) {
    user_function_actual_sensor_ = user_function_actual_sensor;
  }

  // void add_target_temp_callback(std::function<void(float)> &&callback);
  // void add_fan_speed_callback(std::function<void(int)> &&callback);
  // void add_operation_mode_callback(std::function<void(int)>&& callback);

  //void loop() override;
  //void update() override;

  void handleData(const std::vector<uint8_t>& data);
  void handleDeviceInputData(const std::vector<uint8_t>& data);
  void handleDiscreteIOInputData(const std::vector<uint8_t>& data);
  void handleUserFunctionsHoldingData(const std::vector<uint8_t>& data);  
  void handleControlInputData(const std::vector<uint8_t>& data);
  void handleAirflowInputData(const std::vector<uint8_t>& data);
  void handleAirtempInputData(const std::vector<uint8_t>& data);
  void handleCentralHeatInputData(const std::vector<uint8_t>& data);
  void handleUserPanelInputData(const std::vector<uint8_t>& data);
  
  void handleDiscreteIOHoldingData(const std::vector<uint8_t>& data);

  void publishState(binary_sensor::BinarySensor *sensor, bool value) {
    if (sensor &&
        ((sensor->state != value) ||
         ignore_previous_state_)) sensor->publish_state(value);
  }

  void publishState(text_sensor::TextSensor *sensor, const std::string& value) {
    if (sensor &&
        ((sensor->state != value) ||
         ignore_previous_state_)) sensor->publish_state(value);
  }

  void writeTargetTemperature(float new_target_temp);
  void writeFanMode(int new_fan_speed);
  void writeOperationMode(int new_mode);
  void writeRunset(int new_mode);
  void writeDataIgnoreResponse(uint16_t register_address, int write_data);

  void dump_config() override;

protected:
  const std::vector<ReadRegister>enabled_read_registers_ = {
    ReadRegister::device_input,
    ReadRegister::discrete_io_input,
    ReadRegister::discrete_io_holding,
    ReadRegister::user_functions_holding,
    ReadRegister::control_input,
    ReadRegister::airtemp_input,
  };


  ReadWriteMode current_read_write_mode_                = { ReadWriteMode::read };
  std::vector<ReadRegister>::const_iterator read_state_ =
  { enabled_read_registers_.begin() };
  std::deque<WriteableData>writequeue_;
  bool waiting_{ false };
  long last_send_{ 0 };

  binary_sensor::BinarySensor *on_off_state_sensor_;
  binary_sensor::BinarySensor *is_summer_sensor_;
  binary_sensor::BinarySensor *filter_ok_sensor_;
  binary_sensor::BinarySensor *door_open_sensor_;
  binary_sensor::BinarySensor *bypass_on_off_sensor_;
  binary_sensor::BinarySensor *airflap_sensor_;
  binary_sensor::BinarySensor *smokeflap_sensor_;
  binary_sensor::BinarySensor *aircirc_pump_sensor_;
  binary_sensor::BinarySensor *airheat_allow_sensor_;
  binary_sensor::BinarySensor *airheat_1_sensor_;
  binary_sensor::BinarySensor *airheat_2_sensor_;
  binary_sensor::BinarySensor *airheat_3_sensor_;
  binary_sensor::BinarySensor *compressor_sensor_;
  binary_sensor::BinarySensor *compressor_2_sensor_;
  binary_sensor::BinarySensor *four_way_cool_sensor_;
  binary_sensor::BinarySensor *hotgas_heat_sensor_;
  binary_sensor::BinarySensor *hotgas_cool_sensor_;
  binary_sensor::BinarySensor *condenser_open_sensor_;
  binary_sensor::BinarySensor *condenser_close_sensor_;
  binary_sensor::BinarySensor *water_heat_sensor_;
  binary_sensor::BinarySensor *three_way_valve_sensor_;

  text_sensor::TextSensor *version_info_sensor_;
  text_sensor::TextSensor *control_state_sensor_;
  text_sensor::TextSensor *operation_mode_sensor_;
  text_sensor::TextSensor *display_line1_sensor_;
  text_sensor::TextSensor *display_line2_sensor_;
  text_sensor::TextSensor *user_function_actual_sensor_;
  select::Select *user_function_select_;

  CallbackManager<void(float)>target_temp_callback_;
  CallbackManager<void(int)>fan_speed_callback_;
  CallbackManager<void(int)>operation_mode_callback_;

private:
  void writeModbusRegister(WriteableData write_data);
  uint16_t get_16bit(const std::vector<uint8_t>& data, size_t i) {
    return (uint16_t(data[i]) << 8) | uint16_t(data[i + 1]);
  }

  float scaleAndConvertToFloat(uint16_t rawValue) {
    return static_cast<int16_t>(rawValue) / 100.0;
  }

  bool ignore_previous_state_ = true;
};
} // namespace nilan
} // namespace esphome
