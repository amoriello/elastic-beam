// Copyright (c) 2006-2008 The Elastic-Beam Authors. Beerware License.


#ifndef SRC_POWER_CTL_HPP_
#define SRC_POWER_CTL_HPP_

#include "products.h"

struct libusb_context;
struct libusb_device;
struct libusb_device_handle;

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

  bool UsbSendCommand(uint8_t outlet, uint8_t request_type, uint8_t request);

private:
  const PowerStrip& product_;
  libusb_context* p_ctx_ = nullptr;
  libusb_device_handle* p_hdev_ = nullptr;
};

#endif  // SRC_POWER_CTL_HPP_
