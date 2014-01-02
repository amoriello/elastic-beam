// Copyright (c) 2006-2008 The Elastic-Beam Authors. Beerware License.


#ifndef SRC_POWER_CTL_H_
#define SRC_POWER_CTL_H_

#include <cstdint>

struct libusb_context;
struct libusb_device;
struct libusb_device_handle;
struct PowerStrip;



class PowerCtl {
 public:
  explicit PowerCtl(const PowerStrip& product);

  ~PowerCtl();

 public:
  bool Init();

 public:
  void TurnOn(uint8_t outlet);
  void TurnOff(uint8_t outlet);

  bool IsOn(uint8_t outlet) const;
  bool IsOff(uint8_t outlet) const;

  void Toogle(uint8_t outlet);
  void PrintSerial() const;


 private:
  struct UsbMessage {
    uint8_t   request_type = 0;
    uint8_t   request = 0;
    uint16_t  value = 0;
    uint16_t  index = 0;
    uint8_t*  data = 0;
    uint8_t   data_size = 0;
    uint32_t  timeout = 500;
  };

  enum OutletState {
    kOff = 0x00,
    kOn = 0x03
  };


  bool SetOutletState(uint8_t outlet, OutletState action);
  bool GetOutletState(uint8_t outlet) const;

  int EnumDevices() const;
  void PrintDevice(libusb_device* p_device) const;

  int UsbSendCommand(UsbMessage* p_message) const;

 private:
  const PowerStrip& product_;
  libusb_context* p_ctx_ = nullptr;
  libusb_device_handle* p_hdev_ = nullptr;
};

#endif  // SRC_POWER_CTL_H_
