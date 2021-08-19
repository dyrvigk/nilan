#pragma once

#include "../nilan.h"
#include "esphome/core/component.h"
#include "esphome/components/select/select.h"

namespace esphome {
namespace nilan {
class NilanSelect : public select::Select, public Component {
public:

  NilanSelect(Nilan *nilan) : nilan_(nilan) {}

protected:
  Nilan *nilan_;
  /// Override control to change settings of the climate device.
  void control(const std::string &value) override;
};
} // namespace nilan
} // namespace esphome
