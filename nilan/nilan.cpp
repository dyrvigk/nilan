#include "nilan.h"
#include "esphome/core/log.h"

namespace esphome {
namespace nilan {

static const char *TAG = "nilan";

static const uint16_t RUNSET = 1000;
static const uint16_t MODESET = 1001;
static const uint16_t VENTSET = 1002;
static const uint16_t TEMPSET = 1003;

static const uint8_t CMD_READ_INPUT_REG = 4;
static const uint8_t CMD_READ_HOLDING_REG = 3;
static const uint8_t CMD_WRITE_MULTIPLE_REG = 16;

uint16_t crc16(const uint8_t *data, uint8_t len) {
  uint16_t crc = 0xFFFF;
  while (len--) {
    crc ^= *data++;
    for (uint8_t i = 0; i < 8; i++) {
      if ((crc & 0x01) != 0) {
        crc >>= 1;
        crc ^= 0xA001;
      } else {
        crc >>= 1;
      }
    }
  }
  return crc;
}

void Nilan::add_target_temp_callback(std::function<void(float)> &&callback) { target_temp_callback_.add(std::move(callback)); }
void Nilan::add_fan_speed_callback(std::function<void(int)> &&callback) { fan_speed_callback_.add(std::move(callback)); }

void Nilan::handleTemperatureData(const std::vector<uint8_t> &data) {
  if(data.size() != 46) {
    ESP_LOGD(TAG, "Temperature Data has wrong size!!! %s", hexencode(data).c_str());
    return;
  }
  
  //ESP_LOGD(TAG, "Temperature Data: %s", hexencode(data).c_str());
  
  // Temperatures
  auto raw_16 = get_16bit(data, 0);
  float t0 = scaleAndConvertToFloat(raw_16);
  raw_16 = get_16bit(data, 6);
  float t3 = scaleAndConvertToFloat(raw_16);
  raw_16 = get_16bit(data, 8);
  float t4 = scaleAndConvertToFloat(raw_16);
  raw_16 = get_16bit(data, 14);
  float t7 = scaleAndConvertToFloat(raw_16);
  raw_16 = get_16bit(data, 16);
  float t8 = scaleAndConvertToFloat(raw_16);
  raw_16 = get_16bit(data, 30);
  float t15 = scaleAndConvertToFloat(raw_16);
  raw_16 = get_16bit(data, 42);
  float humidity = scaleAndConvertToFloat(raw_16);
  auto co2_level = get_16bit(data, 44);

  //ESP_LOGD(TAG, "NILAN Temperature: T0=%.1f °C, T3=%.1f °C, T4=%.1f °C, T7=%.1f °C, T8=%.1f °C, T15=%.1f °C", t0, t3, t4, t7, t8, t15);
  
  // Temperatures
  publishState(this->temp_t0_sensor_, t0);
  publishState(this->temp_t3_sensor_, t3);
  publishState(this->temp_t4_sensor_, t4);
  publishState(this->temp_t7_sensor_, t7);
  publishState(this->temp_t8_sensor_, t8);
  publishState(this->temp_t15_sensor_, t15);
  publishState(this->measured_humidity_sensor_, humidity);
  publishState(this->co2_sensor_, co2_level);
}

void Nilan::handleAlarmData(const std::vector<uint8_t> &data) {
  if(data.size() != 20) {
    ESP_LOGD(TAG, "Alarm Data has wrong size!!! %s", hexencode(data).c_str());
    return;
  }
  
  //ESP_LOGD(TAG, "Alarm Data: %s", hexencode(data).c_str());
  
  auto alarm_count = get_16bit(data, 0);
  publishState(this->active_alarms_sensor_, alarm_count);
}

void Nilan::handleSpecificAlarms(const std::vector<uint8_t> &data) {
  if(data.size() != 28) {
    ESP_LOGD(TAG, "Specific Alarm data has wrong size!!! %s", hexencode(data).c_str());
    return;
  }
  
  //ESP_LOGD(TAG, "Specific Alarm data: %s", hexencode(data).c_str());
  
  auto filter_alarm = get_16bit(data, 2);
  publishState(this->filter_ok_sensor_, !filter_alarm);
   
  auto door_open = get_16bit(data, 4);
  publishState(this->door_open_sensor_, door_open);
}

void Nilan::handleAirtempHoldingData(const std::vector<uint8_t> &data) {
  if(data.size() != 12) {
    ESP_LOGD(TAG, "Airtemp Holding data has wrong size!!! %s", hexencode(data).c_str());
    return;
  }
  
  //ESP_LOGD(TAG, "Airtemp Holding data: %s", hexencode(data).c_str());
  auto value = get_16bit(data, 0);
  publishState(this->cool_target_temp_sensor_, scaleAndConvertToFloat(value));

  value = get_16bit(data, 2);
  publishState(this->min_summer_temp_sensor_, scaleAndConvertToFloat(value));

  value = get_16bit(data, 4);
  publishState(this->min_winter_temp_sensor_, scaleAndConvertToFloat(value));

  value = get_16bit(data, 6);
  publishState(this->max_summer_temp_sensor_, scaleAndConvertToFloat(value));
    
  value = get_16bit(data, 8);
  publishState(this->max_winter_temp_sensor_, scaleAndConvertToFloat(value));
}

void Nilan::handleAirtempInputData(const std::vector<uint8_t> &data) {
  if(data.size() != 14) {
    ESP_LOGD(TAG, "Airtemp Input data has wrong size!!! %s", hexencode(data).c_str());
    return;
  }
  
  //ESP_LOGD(TAG, "Airtemp Input data: %s", hexencode(data).c_str());
  auto value = get_16bit(data, 0);
  publishState(this->is_summer_sensor_, value);

  value = get_16bit(data, 8);
  auto efficiency = scaleAndConvertToFloat(value);
  publishState(this->heat_exchange_efficiency_sensor_, efficiency);
}

void Nilan::handleControlStateInputData(const std::vector<uint8_t> &data) {
  if(data.size() != 8) {
    ESP_LOGD(TAG, "Control state input data has wrong size!!! %s", hexencode(data).c_str());
    return;
  }
  
  //ESP_LOGD(TAG, "Control state input data: %s", hexencode(data).c_str());
  auto value = get_16bit(data, 0);
  publishState(this->on_off_state_sensor_, value);

  value = get_16bit(data, 2);
  if(this->operation_mode_sensor_ != nullptr)
  {
    std::string mode_str;
    switch(value)
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
  if(this->control_state_sensor_ != nullptr)
  {
    std::string state_str;
    switch(value)
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

void Nilan::handleControlStateHoldingData(const std::vector<uint8_t>& data) {
    if (data.size() != 10) {
        ESP_LOGD(TAG, "Control state holding data has wrong size!!! %s", hexencode(data).c_str());
        return;
    }

    ESP_LOGD(TAG, "Control state holding data: %s", hexencode(data).c_str());

    auto value = get_16bit(data, 2);
    //ESP_LOGD(TAG, "User on/off is set to %d", value);

    value = get_16bit(data, 4);
    //ESP_LOGD(TAG, "Operation mode is set to %d", value);

    value = get_16bit(data, 6);
    ESP_LOGD(TAG, "Ventilation step is set to %d", value);

    value = get_16bit(data, 8);
    ESP_LOGD(TAG, "User temperature setpoint is %f", scaleAndConvertToFloat(value));
}

void Nilan::handleFlapsData(const std::vector<uint8_t>& data) {
    if (data.size() != 8) {
        ESP_LOGD(TAG, "Flaps data has wrong size!!! %s", hexencode(data).c_str());
        return;
    }

    //ESP_LOGD(TAG, "Flaps data: %s", hexencode(data).c_str());
    auto bypass_open = get_16bit(data, 4);
    auto bypass_close = get_16bit(data, 6);

    //ESP_LOGD(TAG, "BypassOpen: %d - BypassClose: %d", bypass_open, bypass_close);
    if(this->bypass_on_off_sensor_) {
      if(this->bypass_on_off_sensor_->state && bypass_close) {
        publishState(this->bypass_on_off_sensor_, false);
      }
      else if(!this->bypass_on_off_sensor_->state && bypass_open) {
        publishState(this->bypass_on_off_sensor_, true);
      }
    }
}

void Nilan::handleFanData(const std::vector<uint8_t>& data) {
    if (data.size() != 4) {
        ESP_LOGD(TAG, "Fan data has wrong size!!! %s", hexencode(data).c_str());
        return;
    }

    //ESP_LOGD(TAG, "Flaps data: %s", hexencode(data).c_str());
    auto raw_16 = get_16bit(data, 0);
    float exhaust = scaleAndConvertToFloat(raw_16);
    raw_16 = get_16bit(data, 2);
    float inlet = scaleAndConvertToFloat(raw_16);

    //ESP_LOGD(TAG, "Exhaust: %f - Inlet: %f", exhaust, inlet);
    publishState(this->exhaust_fan_sensor_, exhaust);
    publishState(this->inlet_fan_sensor_, inlet);
}

void Nilan::handleVersionInfoData(const std::vector<uint8_t> &data) {
  if(data.size() != 8) {
    ESP_LOGD(TAG, "Version info data has wrong size!!! %s", hexencode(data).c_str());
    return;
  }

  ESP_LOGD(TAG, "Version info data: %s", hexencode(data).c_str());
  
  char version_cstr[20];
  sprintf(version_cstr, "%c%c%c%c%c%c", 
    //get_16bit(data, 0), // Bus version
    data[3], data[2],
    data[5], data[4],
    data[7], data[6]); // Bus version 8

  // TODO:
  // Bus version 16: skift ikke plads - sæt selv punktummer
  // Eks: 1.6.45 - 00.16.20.31.20.36.34.35

  std::string version_str = version_cstr;
  publishState(this->version_info_sensor_, version_str);
}

void Nilan::on_modbus_data(const std::vector<uint8_t> &data) {
  this->waiting_ = false;

  switch(state_)
  {
    case Nilan::temperatures:
      handleTemperatureData(data);
      state_ = Nilan::alarms;
      break;
    case Nilan::alarms:
      handleAlarmData(data);
      state_ = Nilan::specific_alarms;
      break; 
    case Nilan::specific_alarms:
      handleSpecificAlarms(data);
      state_ = Nilan::airtemp_holding;
      break;    
    case Nilan::airtemp_holding:
      handleAirtempHoldingData(data);
      state_ = Nilan::airtemp_input;
      break;
    case Nilan::airtemp_input:
      handleAirtempInputData(data);
      state_ = Nilan::control_state_input;
      break;
    case Nilan::control_state_input:
      handleControlStateInputData(data);
      state_ = Nilan::control_state_holding;
      break;
    case Nilan::control_state_holding:
      handleControlStateHoldingData(data);
      state_ = Nilan::flaps_data;
      break;
    case Nilan::flaps_data:
      handleFlapsData(data);
      state_ = Nilan::fan_data;
      break;
    case Nilan::fan_data:
      handleFanData(data);
      state_ = Nilan::version_info;
      break;
    case Nilan::version_info:
      handleVersionInfoData(data);
      state_ = Nilan::idle;
      break;
    case Nilan::target_temp_write:
      ESP_LOGD(TAG, "Target temp write response: %s", hexencode(data).c_str());
      state_ = Nilan::idle;
      break;
    case Nilan::fan_write:
      ESP_LOGD(TAG, "Fan mode write response: %s", hexencode(data).c_str());
      state_ = Nilan::idle;
      break;  

    case Nilan::idle:
    default:
      ESP_LOGW(TAG, "Received data, while in idle mode. Should not happen");
      break;
  }
}

void Nilan::loop() {
  long now = millis();
  // timeout after 15 seconds
  if (this->waiting_ && (now - this->last_send_ > 15000)) {
    ESP_LOGW(TAG, "timed out waiting for response");
    this->waiting_ = false;
  }
  if (this->waiting_ /*|| (this->state_ == 0)*/ || (now - this->last_send_ < 1000))
    return;
  this->last_send_ = now;
  //this->send(CMD_FUNCTION_REG, REGISTER_START[this->state_ - 1], REGISTER_COUNT[this->state_ - 1]);
  
  this->waiting_ = true;

  switch(state_) {
    case Nilan::temperatures:
      //ESP_LOGD(TAG, "Reading temperatures");
      this->send(CMD_READ_INPUT_REG, 200, 23);
      break;
    case Nilan::alarms:
      //ESP_LOGD(TAG, "Reading alarms");
      this->send(CMD_READ_INPUT_REG, 400, 10);
      break;
    case Nilan::specific_alarms:
      //ESP_LOGD(TAG, "Reading specific alarms");
      this->send(CMD_READ_INPUT_REG, 100, 14);
      break;
    case Nilan::airtemp_holding:
      //ESP_LOGD(TAG, "Reading airtemp holding");
      this->send(CMD_READ_HOLDING_REG, 1200, 6);
      break;
    case Nilan::airtemp_input:
      //ESP_LOGD(TAG, "Reading airtemp input");
      this->send(CMD_READ_INPUT_REG, 1200, 7);
      break;
    case Nilan::control_state_input:
      //ESP_LOGD(TAG, "Reading control state input");
      this->send(CMD_READ_INPUT_REG, 1000, 4);
      break;
    case Nilan::control_state_holding:
      //ESP_LOGD(TAG, "Reading control state holding");
      this->send(CMD_READ_HOLDING_REG, 1000, 5);
      break;
    case Nilan::flaps_data:
      //ESP_LOGD(TAG, "Reading flaps data");
      this->send(CMD_READ_HOLDING_REG, 100, 4);
      break;
    case Nilan::fan_data:
      //ESP_LOGD(TAG, "Reading fan data");
      this->send(CMD_READ_HOLDING_REG, 200, 2);
      break;
    case Nilan::version_info:
      //ESP_LOGD(TAG, "Reading version info");
      this->send(CMD_READ_INPUT_REG, 0, 4);
      break;
    case Nilan::idle:
    default:
      //ESP_LOGD(TAG, "No reading");
      this->ignore_previous_state_ = false;
      this->waiting_ = false;

      if(this->target_temp_write_value_ != -1) {
        ESP_LOGD(TAG, "Writing target temp");
        this->state_ = Nilan::target_temp_write;
        //this->send(CMD_WRITE_MULTIPLE_REG, TEMPSET, this->target_temp_write_value_);
        
        uint8_t data[11] = {
          address_, 
          CMD_WRITE_MULTIPLE_REG, 
          TEMPSET >> 8, // TEMPSET msb
          TEMPSET & 0xff, // TEMPSET lsb
          0, // Number of registers to write msb
          1, // Number of registers to write lsb
          2, // Number of bytes to come
          (uint8_t)(this->target_temp_write_value_ >> 8),
          (uint8_t)(this->target_temp_write_value_ & 0xff),
          0, 
          0
        };
        uint16_t crc = crc16(data, 9);
        data[9] = crc & 0xff;
        data[10] = crc >> 8;
        parent_->write_array(data, sizeof(data));
        parent_->flush();
        
        this->target_temp_write_value_ = -1;
      }
      else if(this->fan_mode_write_value_ != -1) {
        ESP_LOGD(TAG, "Writing fan mode");
        this->state_ = Nilan::fan_write;
        //this->send(CMD_WRITE_MULTIPLE_REG, VENTSET, this->fan_mode_write_value_);
        //this->send(0x06, VENTSET, this->fan_mode_write_value_);

        uint8_t data[11] = {
          address_, 
          CMD_WRITE_MULTIPLE_REG,
          VENTSET >> 8, // VENTSET msb
          VENTSET & 0xff, // VENTSET lsb
          0, // Number of registers to write msb
          1, // Number of registers to write lsb
          2, // Number of bytes to come
          (uint8_t)(this->fan_mode_write_value_ >> 8),
          (uint8_t)(this->fan_mode_write_value_ & 0xff),
          0, 
          0
        };
        uint16_t crc = crc16(data, 9);
        data[9] = crc & 0xff;
        data[10] = crc >> 8;
        parent_->write_array(data, sizeof(data));
        parent_->flush();

        this->fan_mode_write_value_ = -1;
      }
      break;
  }
}

void Nilan::update() { this->state_ = Nilan::temperatures; }

void Nilan::writeTargetTemperature(float new_target_temp)
{
  this->target_temp_write_value_ = static_cast<uint16_t>(new_target_temp*100);
  ESP_LOGD(TAG, "Target temp write pending.... (%u)", this->target_temp_write_value_);
}

void Nilan::writeFanMode(int new_fan_speed)
{
  this->fan_mode_write_value_ = static_cast<uint16_t>(new_fan_speed);
  ESP_LOGD(TAG, "Fan speed write pending.... (%u)", this->fan_mode_write_value_);
}

void Nilan::dump_config() {
  ESP_LOGCONFIG(TAG, "NILAN:");
  ESP_LOGCONFIG(TAG, "  Address: 0x%02X", this->address_);

  LOG_SENSOR("", "Temp_t0", this->temp_t0_sensor_);
  LOG_SENSOR("", "Temp_t3", this->temp_t3_sensor_);
  LOG_SENSOR("", "Temp_t4", this->temp_t4_sensor_);
  LOG_SENSOR("", "Temp_t7", this->temp_t7_sensor_);
  LOG_SENSOR("", "Temp_t8", this->temp_t8_sensor_);
  LOG_SENSOR("", "Temp_t15", this->temp_t15_sensor_);
  LOG_SENSOR("", "Measured_Humidity", this->measured_humidity_sensor_);
  LOG_SENSOR("", "ActiveAlarms", this->active_alarms_sensor_);
  LOG_SENSOR("", "CoolSetTemp", this->cool_target_temp_sensor_);
  LOG_SENSOR("", "Inlet_Fan", this->inlet_fan_sensor_);
  LOG_SENSOR("", "Extract_Fan", this->exhaust_fan_sensor_);
  //LOG_SENSOR("", "Bypass", this->bypass_on_off_sensor_);
  //LOG_SENSOR("", "Summer mode", this->is_summer_sensor_);
  
  /*
  LOG_SENSOR("", "Target_Temp", this->target_temp_sensor_);
  LOG_SENSOR("", "Speed_Mode", this->speed_mode_sensor_);
  LOG_SENSOR("", "Heat", this->heat_sensor_);
  LOG_SENSOR("", "Timer", this->timer_sensor_);*/
}

}  // namespace nilan
}  // namespace esphome

