// Copyright (c) 2006-2008 The Elastic-Beam Authors. Beerware License".

#include "power_controller/power_ctl.h"
#include "power_controller/products.h"
#include "power_controller/program_options.h"

#include <iostream>


int main(int argc, char** argv) {
  ProgramOptions options(argc, argv);

  if (options.IsEmpty()) {
    return 0;
  }

  const auto& power_strip = products::kSisPmFlashNew;
  PowerCtl power_ctl(power_strip);

  if (!power_ctl.Init()) {
    return 1;
  }

  const auto& outlets_to_power_on = options.GetPowerOnList();
  for (uint8_t outlet : outlets_to_power_on) {
    if (power_strip.IsValidOutlet(outlet)) {
      power_ctl.TurnOn(outlet);
    }
  }

  const auto& outlets_to_power_off = options.GetPowerOffList();
  for (uint8_t outlet : outlets_to_power_off) {
    if (power_strip.IsValidOutlet(outlet)) {
      power_ctl.TurnOff(outlet);
    }
  }

  const auto& outlets_to_toogle = options.GetToggleList();
  for (uint8_t outlet : outlets_to_toogle) {
    if (power_strip.IsValidOutlet(outlet)) {
      power_ctl.Toggle(outlet);
    }
  }

  return 0;
}
