// Copyright (c) 2006-2008 The Elastic-Beam Authors. Beerware License".

#include "power_controller/power_ctl.h"
#include "power_controller/products.h"
#include "power_controller/program_options.h"


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

  for (uint8_t outlet : options.GetPowerOnList()) {
    if (power_strip.IsValidOutlet(outlet)) {
      power_ctl.TurnOn(outlet);
    }
  }

  for (uint8_t outlet : options.GetPowerOffList()) {
    if (power_strip.IsValidOutlet(outlet)) {
      power_ctl.TurnOff(outlet);
    }
  }

  for (uint8_t outlet : options.GetToggleList()) {
    if (power_strip.IsValidOutlet(outlet)) {
      power_ctl.Toggle(outlet);
    }
  }

  return 0;
}
