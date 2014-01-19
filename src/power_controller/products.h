// Copyright (c) 2006-2008 The Elastic-Beam Authors. Beerware License.


#ifndef SRC_POWER_CONTROLLER_PRODUCTS_H_
#define SRC_POWER_CONTROLLER_PRODUCTS_H_

#include <cstdint>

namespace products {

struct PowerStrip {
  uint16_t vendor_id;
  uint16_t product_id;
  uint8_t nb_outlet;

  bool IsValidOutlet(uint8_t outlet) const {
    return (outlet > 0) && (outlet <= nb_outlet);
  }
};

/*
static PowerStrip kSisPm{
  0x04b4,  // vendor_id
  0xfd11,  // product_id
  1        // nb_outlet
};

static PowerStrip kMSisPmOld {
  0x04b4,  // vendor_id
  0xfd11,  // product_id
  1        // nb_outlet
};

static PowerStrip kMSisPmFlash {
  0x04b4,  // vendor_id
  0xfd11,  // product_id
  1
};
*/

static const PowerStrip kSisPmFlashNew {
  0x04b4,   // vendor_id
  0xfd13,   // product_id
  4         // nb_outlet
};
}  // namespace products

#endif  // SRC_POWER_CONTROLLER_PRODUCTS_H_
