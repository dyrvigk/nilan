#pragma once

#include "../nilan.h"
#include "esphome/core/component.h"
#include "esphome/components/number/number.h"

namespace esphome {
namespace nilan {

enum class NilanNumberType {
  USER_TEMP_SET,
  USER_VENT_SET,
  USER_TIME_SET,
};

class NilanNumber : public number::Number, public Component {
public:
  //NilanNumber(Nilan *nilan) : nilan_(nilan) {}
  void set_type(NilanNumberType type);

protected:
  Nilan *nilan_;
  /// Override control to change settings of the climate device.
  void control(float value) override;

private:
  NilanNumberType type_;
  //Nilan *nilan_;
};
} // namespace nilan
} // namespace esphome
