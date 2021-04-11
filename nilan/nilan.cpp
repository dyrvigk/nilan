#include "nilan.h"
#include "esphome/core/log.h"

namespace esphome {
namespace nilan {

#define VENTSET 1003
#define RUNSET 1001
#define MODESET 1002
#define TEMPSET 1004

static const char *TAG = "nilan";

static const uint8_t CMD_READ_INPUT_REG = 0x04;
static const uint8_t CMD_READ_HOLDING_REG = 0x03;
static const uint8_t CMD_WRITE_MULTIPLE_REG = 0x10;
static const uint16_t REGISTER_START[] = {0, 100, 0, 100};
static const uint16_t REGISTER_COUNT[] = {12, 10, 1, 7};
static const uint16_t REGISTER_WRITE[] = {4};

void Nilan::add_target_temp_callback(std::function<void(float)> &&callback) { target_temp_callback_.add(std::move(callback)); }
void Nilan::add_fan_speed_callback(std::function<void(int)> &&callback) { fan_speed_callback_.add(std::move(callback)); }

void Nilan::on_modbus_data(const std::vector<uint8_t> &data) {
	uint32_t raw_32;
	uint16_t raw_16;
  
	auto get_16bit = [&](size_t i) -> uint16_t {
		return (uint16_t(data[i]) << 8) | uint16_t(data[i + 1]);
	};
  
	this->waiting_ = false;
	if (data.size() < REGISTER_COUNT[this->state_ - 1] * 2) {
		ESP_LOGW(TAG, "Invalid data packet size (%d) for state %d", data.size(), this->state_);
		return;
	}
	ESP_LOGD(TAG, "Data: %s", hexencode(data).c_str());

	if (this->state_ == 1) {
		this->state_ = 2;
		
		// Temperatures
		raw_16 = get_16bit(0);
		float t0 = raw_16 / 100.0;
		raw_16 = get_16bit(6);
		float t3 = raw_16 / 100.0;
		raw_16 = get_16bit(8);
		float t4 = raw_16 / 100.0;
		raw_16 = get_16bit(14);
		float t7 = raw_16 / 100.0;
		raw_16 = get_16bit(16);
		float t8 = raw_16 / 100.0;
		raw_16 = get_16bit(30);
		float t15 = raw_16 / 100.0;
    raw_16 = get_16bit(42);
		float humidity = raw_16 / 100.0;

		ESP_LOGD(TAG, "NILAN Temperature: T0=%.1f °C, T3=%.1f °C, T4=%.1f °C, T7=%.1f °C, T8=%.1f °C, T15=%.1f °C", t0, t3, t4, t7, t8, t15);
		
		// Temperatures
		if (this->temp_t0_sensor_ != nullptr)
			this->temp_t0_sensor_->publish_state(t0);
		if (this->temp_t3_sensor_ != nullptr)
			this->temp_t3_sensor_->publish_state(t3);
		if (this->temp_t4_sensor_ != nullptr)
			this->temp_t4_sensor_->publish_state(t4);
		if (this->temp_t7_sensor_ != nullptr)
			this->temp_t7_sensor_->publish_state(t7);
		if (this->temp_t8_sensor_ != nullptr)
			this->temp_t8_sensor_->publish_state(t8);
		if (this->temp_t15_sensor_ != nullptr)
			this->temp_t15_sensor_->publish_state(t15);
		if (this->measured_humidity_sensor_ != nullptr)
			this->measured_humidity_sensor_->publish_state(humidity);

		return;
	}
	/*
	if (this->state_ == 2) {
		this->state_ = 3;
		this->CMD_FUNCTION_REG = 0x03;
		
		raw_16 = get_16bit(2);
		float alarm_bit = raw_16;
		raw_16 = get_16bit(4);
		float inlet_fan = raw_16;
		raw_16 = get_16bit(6);
		float extract_fan = raw_16;
		raw_16 = get_16bit(8);
		float bypass = raw_16;
		raw_16 = get_16bit(10);
		float watervalve = raw_16;
		raw_16 = get_16bit(12);
		float humidity_fan_control = raw_16;
		raw_16 = get_16bit(14);
		float bypass_on_off = raw_16;
		
		ESP_LOGD(TAG, "Alarm: Alarm=%.2f", alarm_bit);
		ESP_LOGD(TAG, "Runinfo: Inlet=%.2f %%, Extract=%.2f %%, Bypass=%.2f %%, Watervalve=%.2f %%, Humidity_fan_control=%.2f %%, Bypass_on_off=%.2f", inlet_fan, extract_fan, bypass, watervalve, humidity_fan_control, bypass_on_off);
		if (this->alarm_bit_sensor_ != nullptr)
			this->alarm_bit_sensor_->publish_state(alarm_bit);
		if (this->inlet_fan_sensor_ != nullptr)
			this->inlet_fan_sensor_->publish_state(inlet_fan);
		if (this->extract_fan_sensor_ != nullptr)
			this->extract_fan_sensor_->publish_state(extract_fan);
		if (this->bypass_sensor_ != nullptr)
			this->bypass_sensor_->publish_state(bypass);
		if (this->watervalve_sensor_ != nullptr)
			this->watervalve_sensor_->publish_state(watervalve);
		if (this->humidity_fan_control_sensor_ != nullptr)
			this->humidity_fan_control_sensor_->publish_state(humidity_fan_control);
		if (this->bypass_on_off_sensor_ != nullptr)
			this->bypass_on_off_sensor_->publish_state(bypass_on_off);
		
		
		return;
	}
	
	if (this->state_ == 3) {
		this->state_ = 4;
		this->CMD_FUNCTION_REG = 0x03;
		
		raw_16 = get_16bit(0);
		float target_temp = (raw_16 + 100) / 10;
		
		ESP_LOGD(TAG, "Target_temp: Target_Temp=%.2f", target_temp);
		if (this->target_temp_sensor_ != nullptr)
			this->target_temp_sensor_->publish_state(target_temp);
		target_temp_callback_.call(target_temp);
		return;
	}
	
	if (this->state_ == 4) {
		this->state_ = 0;
		this->CMD_FUNCTION_REG = 0x04;
		
		raw_16 = get_16bit(0);
		int speed_mode = raw_16;
		raw_16 = get_16bit(4);
		float heat = raw_16;
		raw_16 = get_16bit(12);
		float timer = raw_16;
		
		ESP_LOGD(TAG, "Speed: Speed_Mode=%d, heat=%.2f, timer=%.2f" , speed_mode, heat, timer);
		if (this->speed_mode_sensor_ != nullptr)
			this->speed_mode_sensor_->publish_state(speed_mode);
		fan_speed_callback_.call(speed_mode);
		if (this->heat_sensor_ != nullptr)
			this->heat_sensor_->publish_state(heat);
		if (this->timer_sensor_ != nullptr)
			this->timer_sensor_->publish_state(timer);
		return;
	}*/
}

void Nilan::loop() {
  long now = millis();
  // timeout after 15 seconds
if (this->waiting_ && (now - this->last_send_ > 15000)) {
    ESP_LOGW(TAG, "timed out waiting for response");
    this->waiting_ = false;
  }
  if (this->waiting_ || (this->state_ == 0) || (now - this->last_send_ < 1000))
    return;
  this->last_send_ = now;
  //this->send(CMD_FUNCTION_REG, REGISTER_START[this->state_ - 1], REGISTER_COUNT[this->state_ - 1]);
  
	ESP_LOGD(TAG, "Reading temperatures");
  this->send(CMD_READ_INPUT_REG, 200, 23);
  
  this->waiting_ = true;
}

void Nilan::update() { this->state_ = 1; }

void Nilan::writeTargetTemperature(float new_target_temp)
{
	
	ESP_LOGD(TAG, "Writing new target temp to system.... (%f)",(new_target_temp * 10 - 100));
	this->send(CMD_WRITE_MULTIPLE_REG, TEMPSET, (new_target_temp * 10 - 100));
}

void Nilan::writeFanMode(int new_fan_speed)
{
	
	ESP_LOGD(TAG, "Writing new fan speed to system.... (%i)",new_fan_speed);
	this->send(CMD_WRITE_MULTIPLE_REG, VENTSET, new_fan_speed);
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
  /*LOG_SENSOR("", "Humidity_Calculated_Setpoint", this->humidity_calculated_setpoint_sensor_);
  LOG_SENSOR("", "Alarm_Bit", this->alarm_bit_sensor_);
  LOG_SENSOR("", "Inlet_Fan", this->inlet_fan_sensor_);
  LOG_SENSOR("", "Extract_Fan", this->extract_fan_sensor_);
  LOG_SENSOR("", "Bypass_Sensor", this->bypass_sensor_);
  LOG_SENSOR("", "Watervalve", this->watervalve_sensor_);
  LOG_SENSOR("", "Target_Temp", this->target_temp_sensor_);
  LOG_SENSOR("", "Speed_Mode", this->speed_mode_sensor_);
  LOG_SENSOR("", "Heat", this->heat_sensor_);
  LOG_SENSOR("", "Timer", this->timer_sensor_);*/
}

}  // namespace nilan
}  // namespace esphome
