// Copyright (c) 2006-2008 The Elastic-Beam Authors. Beerware License.

#include "power_ctl.h"
#include "scope_exit.h"
#include "products.h"


int main() {
  PowerCtl power_ctl(products::kSisPmFlashNew);

  if (!power_ctl.Init()) {
    return 1;
  }

  power_ctl.TurnOn(1);

  return 0;
}
