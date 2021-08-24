#pragma once

#include "../nilan.h"
#include "esphome/core/component.h"
#include "esphome/components/number/number.h"

namespace esphome {
namespace nilan {
class NilanNumber : public number::Number, public Component {
public:
  NilanNumber(Nilan *nilan) : nilan_(nilan) {}

protected:
  Nilan *nilan_;
  /// Override control to change settings of the climate device.
  void control(float value) override;
};
} // namespace nilan
} // namespace esphome
