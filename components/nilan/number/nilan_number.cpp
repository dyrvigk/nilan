#include "nilan_number.h"
#include "esphome/core/log.h"
#include "../nilan_types.h"

namespace esphome {
namespace nilan {
static const char *TAG = "nilan.number";

static const uint16_t USER_TIME = 602;
static const uint16_t USER_VENT = 603;
static const uint16_t USER_TEMP = 604;

void NilanNumber::control(float value) {
  ESP_LOGD(TAG, "Nilan Number state: %f", value);
  this->publish_state(value);

  switch(type_){
    case NilanNumberType::USER_TEMP_SET:
      nilan_->writeDataIgnoreResponse(USER_TEMP, value);
      break;
    case NilanNumberType::USER_TIME_SET:
      nilan_->writeDataIgnoreResponse(USER_TIME, value);
      break;
    case NilanNumberType::USER_VENT_SET:
      nilan_->writeDataIgnoreResponse(USER_VENT, value);
      break;
    default:
      ESP_LOGD(TAG, "Unknown number sensor type");
      break;
  }
}

void NilanNumber::set_type(NilanNumberType type) {
  type_ = type;
}
 

} // namespace nilan
} // namespace esphome
