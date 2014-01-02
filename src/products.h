// Copyright (c) 2006-2008 The Elastic-Beam Authors. Beerware License.


#ifndef SRC_PRODUCTS_H_
#define SRC_PRODUCTS_H_

#include <cstdint>

struct VendorId {
  uint16_t value;
};

struct ProductId {
  uint16_t value;
};


struct PowerStrip {
  VendorId vendor_id;
  ProductId product_id;
};


namespace products {
static PowerStrip kSisPm{
  VendorId{ 0x04b4 },
  ProductId{ 0xfd11 }
};


static PowerStrip kMSisPmOld {
  VendorId{ 0x04b4 },
  ProductId{ 0xfd11 }
};

static PowerStrip kMSisPmFlash {
  VendorId{ 0x04b4 },
  ProductId{ 0xfd11 }
};

static const PowerStrip kSisPmFlashNew {
  VendorId{ 0x04b4 },
  ProductId{ 0xfd13 }
};
}  // namespace products

#endif  // SRC_PRODUCTS_H_
