// Copyright (c) 2006-2008 The Elastic-Beam Authors. Beerware License.

#include "power_ctl.h"


int main() {
  aqua::PowerCtl power_ctl(aqua::products::kSisPmFlashNew);

  if (!power_ctl.Init()) {
    return 1;
  }

  power_ctl.TurnOn(1);

  return 0;
}
