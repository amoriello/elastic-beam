// Copyright (c) 2006-2008 The Elastic-Beam Authors. Beerware License.


#ifndef POWER_CTL_HPP_
#define POWER_CTL_HPP_


#include <cstdint>
#include <functional>


class libusb_context;
class libusb_device;
class libusb_device_handle;



namespace aqua {

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


static PowerStrip kSisPm {
  VendorId { 0x04b4 },
  ProductId { 0xfd11 }
};


static PowerStrip kMSisPmOld {
  VendorId { 0x04b4 },
  ProductId { 0xfd11 }
};

static PowerStrip kMSisPmFlash {
  VendorId { 0x04b4 },
  ProductId { 0xfd11 }
};

static const PowerStrip kSisPmFlashNew {
  VendorId { 0x04b4 },
  ProductId { 0xfd13 }
};

}  // products



class OnScopeExit {
  typedef std::function<void ()> Handler;

public:
  explicit OnScopeExit(Handler handler)
  : handler_(handler) { }

  ~OnScopeExit() {
    handler_();
  }

private:
  Handler handler_;
};


class PowerCtl {

public:
  PowerCtl(const PowerStrip& product);

  ~PowerCtl();

public:
  bool Init();

public:
  void TurnOn(uint8_t outlet);
  void TurnOff(uint8_t outlet);
  void Toogle(uint8_t outlet);
  void PrintSerial() const;

private:
  enum OutletAction {
    kOff = 0x00,
    kOn = 0x03
  };

  int EnumDevices();

  void PrintDevice(libusb_device* p_device);

  bool SetOutletState(uint8_t outlet, OutletAction action);


private:
  const PowerStrip& product_;
  libusb_context* p_ctx_ = nullptr;
  libusb_device_handle* p_hdev_ = nullptr;
};

}  // aqua


#endif  // POWER_CTL_HPP_
