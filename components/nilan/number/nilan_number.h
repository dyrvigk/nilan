#pragma once

#include "../nilan.h"
#include "esphome/core/component.h"
#include "esphome/components/number/number.h"

namespace esphome {
namespace nilan {

class NilanNumber : public number::Number, public Component {
public:
  NilanNumber(Nilan *nilan) : nilan_(nilan) {}
  void set_type(NilanNumberType type);

protected:
  /// Override control to change settings of the climate device.
  void control(float value) override;

private:
  Nilan *nilan_;
  NilanNumberType type_;
};
} // namespace nilan
} // namespace esphome
