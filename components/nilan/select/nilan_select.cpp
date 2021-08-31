#include "nilan_select.h"
#include "esphome/core/log.h"

namespace esphome {
namespace nilan {
static const char *TAG = "nilan.select";

static const uint16_t USER_FUNC = 601;

void NilanSelect::control(const std::string &value) {
  ESP_LOGD(TAG, "Nilan Select state: %s", value.c_str());
  int val = value[0] - '0'; // Parse char number to int
  this->publish_state(value);

  nilan_->writeDataIgnoreResponse(USER_FUNC, val);
}

} // namespace nilan
} // namespace esphome
