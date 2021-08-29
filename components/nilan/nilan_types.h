#pragma once

namespace esphome {
namespace nilan {

struct WriteableData
{
  uint16_t register_value;
  uint16_t write_value;
};

enum class ReadRegister {
  device_input,
  discrete_io_input,
  analog_io_input,
  alarm_input,
  user_functions_holding,
  control_input,
  airflow_input,
  airtemp_input,
  central_heat_input,
  user_panel_input,
  time_holding,
  airtemp_holding,
  control_state_holding,
  flaps_data,
  fan_data
};

enum class ReadWriteMode {
  read,
  write,
  idle
};

enum class NilanNumberType {
  UNKNOWN,
  USER_TEMP_SET,
  USER_VENT_SET,
  USER_TIME_SET,
};

} // namespace nilan
} // namespace esphome
