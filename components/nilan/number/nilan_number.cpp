#include "nilan_number.h"
#include "esphome/core/log.h"

namespace esphome {
namespace nilan {
static const char *TAG = "nilan.number";

void NilanNumber::control(float value) {
  ESP_LOGD(TAG, "Nilan Number state: %f", value);
}

} // namespace nilan
} // namespace esphome
