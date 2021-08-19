#include "nilan_select.h"
#include "esphome/core/log.h"

namespace esphome {
namespace nilan {
static const char *TAG = "nilan.select";

void NilanSelect::control(const std::string &value) {
  ESP_LOGD(TAG, "Nilan Select state: %s", value.c_str());
}

} // namespace nilan
} // namespace esphome
