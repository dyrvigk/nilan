#include "nilan.h"
#include "esphome/core/log.h"

namespace esphome {
namespace nilan {
static const char *TAG = "nilan";

static const uint16_t RUNSET  = 1001;
static const uint16_t MODESET = 1002;
static const uint16_t VENTSET = 1003;
static const uint16_t TEMPSET = 1004;

static const uint8_t CMD_READ_INPUT_REG     = 4;
static const uint8_t CMD_READ_HOLDING_REG   = 3;
static const uint8_t CMD_WRITE_MULTIPLE_REG = 16;

uint16_t crc16(const uint8_t *data, uint8_t len) {
  uint16_t crc = 0xFFFF;

  while (len--) {
    crc ^= *data++;

    for (uint8_t i = 0; i < 8; i++) {
      if ((crc & 0x01) != 0) {
        crc >>= 1;
        crc  ^= 0xA001;
      } else {
        crc >>= 1;
      }
    }
  }
  return crc;
}

// void Nilan::add_target_temp_callback(std::function<void(float)> &&callback) {
// target_temp_callback_.add(std::move(callback)); }
// void Nilan::add_fan_speed_callback(std::function<void(int)> &&callback) {
// fan_speed_callback_.add(std::move(callback)); }
// void Nilan::add_operation_mode_callback(std::function<void(int)>&& callback)
// { operation_mode_callback_.add(std::move(callback)); }

void on_number_changed(NilanNumberType type, float new_value)
{
  switch(type){
    case NilanNumberType::USER_TEMP_SET:
      break;
    case NilanNumberType::USER_VENT_SET:
      break;
    case NilanNumberType::USER_TIME_SET:
      break;
    default:
      break;
  }
}

void Nilan::handleData(const std::vector<uint8_t>& data)
{
  switch (*this->read_state_)
  {
  case ReadRegister::device_input:
    handleDeviceInputData(data);
    break;

  case ReadRegister::discrete_io_input:
    handleDiscreteIOInputData(data);
    break;

  case ReadRegister::discrete_io_holding:
    handleDiscreteIOHoldingData(data);
    break;

  case ReadRegister::user_functions_holding:
    handleUserFunctionsHoldingData(data);
    break;

  case ReadRegister::control_input:
    handleControlInputData(data);
    break;

  case ReadRegister::airflow_input:
    handleAirflowInputData(data);
    break;

  case ReadRegister::airtemp_input:
    handleAirtempInputData(data);
    break;

  case ReadRegister::central_heat_input:
    handleCentralHeatInputData(data);
    break;

  case ReadRegister::user_panel_input:
    handleUserPanelInputData(data);
    break;

  default:
    ESP_LOGW(TAG, "Received data, in unhandled mode. Should not happen");
    break;
  }
}

void Nilan::handleDeviceInputData(const std::vector<uint8_t>& data) {
  if (data.size() != 8) {
    ESP_LOGD(TAG,
             "Device input data has wrong size!!! %s",
             hexencode(data).c_str());
    return;
  }

  // ESP_LOGD(TAG, "Device input data: %s", hexencode(data).c_str());

  auto bus_version = get_16bit(data, 0);
  char version_cstr[20];

  // For some reason, the version number is stored differently on older systems
  // with bus version 8
  if (bus_version == 8)
  {
    sprintf(version_cstr, "%c%c%c%c%c%c",
            data[3], data[2],
            data[5], data[4],
            data[7], data[6]);
  }

  // Verified with bus version 16
  else
  {
    sprintf(version_cstr, "%c%c.%c%c.%c%c",
            data[2], data[3],
            data[4], data[5],
            data[6], data[7]);
  }

  std::string version_str = version_cstr;

  publishState(this->version_info_sensor_, version_str);
}

void Nilan::handleDiscreteIOInputData(const std::vector<uint8_t>& data) {
  if (data.size() != 32) {
    ESP_LOGD(TAG, "Discrete IO input data has wrong size!!! %s",
             hexencode(data).c_str());
    return;
  }

  // ESP_LOGD(TAG, "Discrete IO data: %s", hexencode(data).c_str());

  auto filter_alarm = get_16bit(data, 2);

  publishState(this->filter_ok_sensor_, !filter_alarm);

  auto door_open = get_16bit(data, 4);

  publishState(this->door_open_sensor_, door_open);
}

void Nilan::handleUserFunctionsHoldingData(const std::vector<uint8_t>& data) {
  if (data.size() != 12) {
    ESP_LOGD(TAG, "User function holding data has wrong size!!! %s",
             hexencode(data).c_str());
    return;
  }

  ESP_LOGD(TAG, "User function holding data: %s", hexencode(data).c_str());

  auto userFuncAct = get_16bit(data, 0);
  std::string actual_user_function;
  switch(userFuncAct) {
    case 0:
      actual_user_function = "0 : None";
      break;
    case 1:
      actual_user_function = "1 : Extend";
      break;
    case 2:
      actual_user_function = "2 : Inlet";
      break;
    case 3:
      actual_user_function = "3 : Exhaust";
      break;
    case 4:
      actual_user_function = "4 : External heater offset";
      break;
    case 5:
      actual_user_function = "5 : Ventilate";
      break;
    case 6:
      actual_user_function = "6 : Cooker Hood";
      break;
  }

  publishState(user_function_actual_sensor_, actual_user_function);

  auto userFuncSet = get_16bit(data, 2);
  auto userTimeSet = get_16bit(data, 4);
  auto userVentSet = get_16bit(data, 6);
  auto userTempSet = get_16bit(data, 8);
  auto userOffsSet = get_16bit(data, 10);
}

void Nilan::handleControlInputData(const std::vector<uint8_t>& data) {
  if (data.size() != 8) {
    ESP_LOGD(TAG,
             "Control input data has wrong size!!! %s",
             hexencode(data).c_str());
    return;
  }

  // ESP_LOGD(TAG, "Control input data: %s", hexencode(data).c_str());

  auto value = get_16bit(data, 0);

  publishState(this->on_off_state_sensor_, value);

  value = get_16bit(data, 2);

  if (this->operation_mode_sensor_ != nullptr)
  {
    std::string mode_str;

    switch (value)
    {
    case 0:
      mode_str = "Off";
      break;

    case 1:
      mode_str = "Heat";
      break;

    case 2:
      mode_str = "Cool";
      break;

    case 3:
      mode_str = "Auto";
      break;

    case 4:
      mode_str = "Service";
      break;

    default:
      mode_str = "Unknown";
      break;
    }

    publishState(this->operation_mode_sensor_, mode_str);
  }

  value = get_16bit(data, 4);

  if (this->control_state_sensor_ != nullptr)
  {
    std::string state_str;

    switch (value)
    {
    case 0:
      state_str = "Off";
      break;

    case 1:
      state_str = "Shift";
      break;

    case 2:
      state_str = "Stop";
      break;

    case 3:
      state_str = "Start";
      break;

    case 4:
      state_str = "Standby";
      break;

    case 5:
      state_str = "Ventilation stop";
      break;

    case 6:
      state_str = "Ventilation";
      break;

    case 7:
      state_str = "Heating";
      break;

    case 8:
      state_str = "Cooling";
      break;

    case 9:
      state_str = "Hot water";
      break;

    case 10:
      state_str = "Legionella";
      break;

    case 11:
      state_str = "Cooling + hot water";
      break;

    case 12:
      state_str = "Central heating";
      break;

    case 13:
      state_str = "Defrost";
      break;

    default:
      state_str = "Unknown";
      break;
    }

    publishState(this->control_state_sensor_, state_str);
  }
}

void Nilan::handleAirflowInputData(const std::vector<uint8_t>& data) {
  if (data.size() != 10) {
    ESP_LOGD(TAG,
             "Airflow input data has wrong size!!! %s",
             hexencode(data).c_str());
    return;
  }

  ESP_LOGD(TAG, "Airflow input data: %s", hexencode(data).c_str());
}

void Nilan::handleAirtempInputData(const std::vector<uint8_t>& data) {
  if (data.size() != 14) {
    ESP_LOGD(TAG,
             "Airtemp input data has wrong size!!! %s",
             hexencode(data).c_str());
    return;
  }

  // ESP_LOGD(TAG, "Airtemp input data: %s", hexencode(data).c_str());

  auto value = get_16bit(data, 0);

  publishState(this->is_summer_sensor_, value);
}

void Nilan::handleCentralHeatInputData(const std::vector<uint8_t>& data) {
  if (data.size() != 2) {
    ESP_LOGD(TAG, "Central heat input data has wrong size!!! %s",
             hexencode(data).c_str());
    return;
  }

  // ESP_LOGD(TAG, "Central heat input data: %s", hexencode(data).c_str());

  auto value = get_16bit(data, 0);
  auto external_heat_setpoint = scaleAndConvertToFloat(value);
  // ESP_LOGD(TAG, "External heat setpoint data: %f", external_heat_setpoint);
  // TODO: Make sensor
}

void Nilan::handleUserPanelInputData(const std::vector<uint8_t>& data) {
  if (data.size() != 24) {
    ESP_LOGD(TAG, "User panel input data has wrong size!!! %s",
             hexencode(data).c_str());
    return;
  }

  ESP_LOGD(TAG, "User panel input data: %s", hexencode(data).c_str());

  // bool led_1_state = static_cast<bool>(get_16bit(data, 0));
  // bool led_2_state = static_cast<bool>(get_16bit(data, 2));
  // char display_line[8];

  /*std::string first_line = { data[5], data[4], data[7], data[6], data[9], data[8], data[11], data[10] };
  std::string second_line = { data[15], data[14], data[17], data[16], data[19], data[18], data[21], data[20] };

  for( auto & character : first_line)
  {
    character = (character == 9 ? 'Ø' : character);
    character = (character == 223 ? '°' : character);
    character = (character == 62 ? '>' : character);
    character = (character == 60 ? '<' : character);
  }
  
  for( auto & character : second_line)
  {
    character = (character == 9 ? 'Ø' : character);
    character = (character == 223 ? 'x' : character);
    character = (character == 62 ? 'y' : character);
    character = (character == 60 ? 'z' : character);
  }
  publishState(display_line1_sensor_, first_line);
  publishState(display_line2_sensor_, second_line);
  */
  // ESP_LOGD(TAG, "User panel first line: %s", first_line.c_str());
  // ESP_LOGD(TAG, "User panel second line: %s", second_line.c_str());
}

void Nilan::handleDiscreteIOHoldingData(const std::vector<uint8_t>& data) {
  if (data.size() != 56) {
    ESP_LOGD(TAG, "Discrete IO holding data has wrong size!!! %s",
             hexencode(data).c_str());
    return;
  }

  // ESP_LOGD(TAG, "Discrete IO holding data: %s", hexencode(data).c_str());

  // Bypass values need special treatment:
  auto bypass_open  = get_16bit(data, 4);
  auto bypass_close = get_16bit(data, 6);

  if (this->bypass_on_off_sensor_) {
    if (this->bypass_on_off_sensor_->state && bypass_close) {
      publishState(this->bypass_on_off_sensor_, false);
    }
    else if (!this->bypass_on_off_sensor_->state && bypass_open) {
      publishState(this->bypass_on_off_sensor_, true);
    }
  }

  // Simple values:
  auto value = get_16bit(data, 0);
  publishState(this->airflap_sensor_, value);
  value = get_16bit(data, 2);
  publishState(this->smokeflap_sensor_, value);
  value = get_16bit(data, 8);
  publishState(this->aircirc_pump_sensor_, value);
  value = get_16bit(data, 10);
  publishState(this->airheat_allow_sensor_, value);
  value = get_16bit(data, 12);
  publishState(this->airheat_1_sensor_, value);
  value = get_16bit(data, 14);
  publishState(this->airheat_2_sensor_, value);
  value = get_16bit(data, 16);
  publishState(this->airheat_3_sensor_, value);
  value = get_16bit(data, 18);
  publishState(this->compressor_sensor_, value);
  value = get_16bit(data, 20);
  publishState(this->compressor_2_sensor_, value);
  value = get_16bit(data, 22);
  publishState(this->four_way_cool_sensor_, value);
  value = get_16bit(data, 24);
  publishState(this->hotgas_heat_sensor_, value);
  value = get_16bit(data, 26);
  publishState(this->hotgas_cool_sensor_, value);
  value = get_16bit(data, 28);
  publishState(this->condenser_open_sensor_, value);
  value = get_16bit(data, 30);
  publishState(this->condenser_close_sensor_, value);
  value = get_16bit(data, 32);
  publishState(this->water_heat_sensor_, value);
  value = get_16bit(data, 34);
  publishState(this->three_way_valve_sensor_, value);
}

void Nilan::writeTargetTemperature(float new_target_temp)
{
  WriteableData data;

  data.write_value    = static_cast<uint16_t>(new_target_temp * 100);
  data.register_value = TEMPSET;

  writequeue_.emplace_back(data);
  ESP_LOGD(TAG, "Target temp write pending.... (%i)", data.write_value);
}

void Nilan::writeFanMode(int new_fan_speed)
{
  WriteableData data;

  data.write_value    = static_cast<uint16_t>(new_fan_speed);
  data.register_value = VENTSET;

  writequeue_.emplace_back(data);
  ESP_LOGD(TAG, "Fan speed write pending.... (%i)", data.write_value);
}

void Nilan::writeOperationMode(int new_mode)
{
  WriteableData data;

  data.ignore_response = false;
  data.write_value    = static_cast<uint16_t>(new_mode);
  data.register_value = MODESET;

  writequeue_.emplace_back(data);
  ESP_LOGD(TAG, "Operation mode write pending.... (%i)", data.write_value);
}

void Nilan::writeRunset(int new_mode)
{
  WriteableData data;

  data.ignore_response = false;
  data.write_value    = static_cast<uint16_t>(new_mode);
  data.register_value = RUNSET;

  writequeue_.emplace_back(data);
  ESP_LOGD(TAG, "Runset write pending.... (%i)", data.write_value);
}

void Nilan::writeDataIgnoreResponse(uint16_t register_address, int write_data)
{
  WriteableData data;

  data.ignore_response = true;
  data.write_value    = static_cast<uint16_t>(write_data);
  data.register_value = register_address;

  writequeue_.emplace_back(data);
  ESP_LOGD(TAG, "Data write pending.... (%i)", data.write_value);
}

void Nilan::writeModbusRegister(WriteableData write_data)
{
  ESP_LOGD(TAG,
           "Writing %d to address %d",
           write_data.write_value,
           write_data.register_value);

  uint8_t  data[11] = {
    address_,
    CMD_WRITE_MULTIPLE_REG,
    (uint8_t)(write_data.register_value >> 8),   // VENTSET msb
    (uint8_t)(write_data.register_value & 0xff), // VENTSET lsb
    0,                                           // Number of registers to write msb
    1,                                           // Number of registers to write lsb
    2,                                           // Number of bytes to come
    (uint8_t)(write_data.write_value >> 8),
    (uint8_t)(write_data.write_value & 0xff),
    0,
    0
  };
  uint16_t crc = crc16(data, 9);

  data[9]  = (uint8_t)(crc & 0xff);
  data[10] = (uint8_t)(crc >> 8);
  parent_->write_array(data, sizeof(data));
  parent_->flush();

  this->waiting_ = !write_data.ignore_response;
}

void Nilan::dump_config() {
  ESP_LOGCONFIG(TAG, "NILAN:");
  ESP_LOGCONFIG(TAG, "  Address: 0x%02X", this->address_);
}
} // namespace nilan
} // namespace esphome
