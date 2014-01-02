// Copyright (c) 2006-2008 The Elastic-Beam Authors. Beerware License.

#include "power_ctl.h"

#include <stdio.h>
#include <libusb.h>

#include "products.h"
#include "scope_exit.h"


//-----------------------------------------------------------------------------
PowerCtl::PowerCtl(const PowerStrip& product)
  : product_(product) { }


//-----------------------------------------------------------------------------
PowerCtl::~PowerCtl() {
  if (p_hdev_) {
    ::libusb_close(p_hdev_);
  }

  if (p_ctx_) {
    ::libusb_exit(p_ctx_);
  }
}


//-----------------------------------------------------------------------------
void PowerCtl::TurnOn(uint8_t outlet) {
  SetOutletState(outlet, kOn);
}


//-----------------------------------------------------------------------------
void PowerCtl::TurnOff(uint8_t outlet) {
  SetOutletState(outlet, kOff);
}


//-----------------------------------------------------------------------------
bool PowerCtl::IsOn(uint8_t outlet) const {
  return GetOutletState(outlet);
}


//-----------------------------------------------------------------------------
bool PowerCtl::IsOff(uint8_t outlet) const {
  return !IsOn(outlet);
}


//-----------------------------------------------------------------------------
void PowerCtl::Toogle(uint8_t outlet) {
  if (IsOn(outlet)) {
    TurnOff(outlet);
  } else {
    TurnOn(outlet);
  }
}

//-----------------------------------------------------------------------------
bool  PowerCtl::Init() {
  libusb_device_handle* p_hdev = nullptr;
  libusb_context* p_ctx = nullptr;
  int ret = 0;

  OnScopeExit cleanup([&]() {
      if (p_hdev) { ::libusb_close(p_hdev); }
      if (p_ctx) { ::libusb_exit(p_ctx); }
    });


  if ((ret = ::libusb_init(&p_ctx)) != 0) {
    fprintf(stderr, "[!] Cannot initialize usb context: %s\n",
                    ::libusb_error_name(ret));
    return !!ret;
  }

  ::libusb_set_debug(p_ctx, 3);

  p_hdev = ::libusb_open_device_with_vid_pid(p_ctx,
                                             product_.vendor_id.value,
                                             product_.product_id.value);

  if (!p_hdev) {
    fprintf(stderr, "[!] Cannot open device\n");
    return false;
  }

  if (::libusb_kernel_driver_active(p_hdev, 0)) {
    fprintf(stdout, "[+] Kernel driver is attached to device\n");
    fprintf(stdout, "[+] Detaching...\n");
    if (::libusb_detach_kernel_driver(p_hdev, 0) == 0) {
      fprintf(stdout, "[+] Detached\n");
    } else {
      fprintf(stderr, "[!] Cannot detach Kernel driver\n");
      return false;
    }
  }


  if ((ret = ::libusb_set_configuration(p_hdev, 1)) != 0) {
    fprintf(stderr, "[!] Cannot set usb configuration for device\n",
                    ::libusb_error_name(ret));
    return false;
  }

  if ((ret = ::libusb_claim_interface(p_hdev, 0)) != 0) {
    fprintf(stderr, "[!] Cannot claim interface for device: %s\n",
                    ::libusb_error_name(ret));
    return false;
  }

  if ((ret = ::libusb_set_interface_alt_setting(p_hdev, 0, 0)) != 0) {
    fprintf(stderr, "[!] Cannot set alternate interface for device: %s\n",
                    ::libusb_error_name(ret));
    return false;
  }

  p_hdev_ = p_hdev;
  p_ctx_ = p_ctx;

  p_hdev = nullptr;
  p_ctx = nullptr;

  return true;
}


//-----------------------------------------------------------------------------
int PowerCtl::EnumDevices() const {
  libusb_device** devices = nullptr;
  auto device_count = ::libusb_get_device_list(p_ctx_, &devices);

  if (device_count < 0) {
    return -1;
  }

  for (auto i = 0; i < device_count; ++i) {
    PrintDevice(devices[i]);
  }

  ::libusb_free_device_list(devices, 1);

  return 0;
}


//-----------------------------------------------------------------------------
void PowerCtl::PrintSerial() const {
  uint8_t buffer[5] = {0, 0, 0, 0, 0};
  UsbMessage message;

  message.request_type = LIBUSB_REQUEST_TYPE_CLASS |
                         LIBUSB_RECIPIENT_INTERFACE |
                         LIBUSB_ENDPOINT_IN;  // 0xa1
  message.request = 0x01;
  message.value = (0x03 << 8) | 0x01;
  message.index = 0;
  message.data = buffer;
  message.data_size = sizeof(buffer);

  int ret = 0;
  if ((ret = UsbSendCommand(&message)) < 0) {
    fprintf(stderr, "[!] Libusb error string: %s\nTerminating\n",
                    ::libusb_error_name(ret));
  }

  fprintf(stdout, "Product Serial : %02x:%02x:%02x:%02x:%02x\n",
          buffer[0],
          buffer[1],
          buffer[2],
          buffer[3],
          buffer[4]);
}


//-----------------------------------------------------------------------------
bool PowerCtl::SetOutletState(uint8_t outlet, OutletState action) {
  uint8_t hw_outlet = 3 * outlet;
  uint8_t buffer[2] = { hw_outlet, action };
  UsbMessage message;

  message.request_type = LIBUSB_REQUEST_TYPE_CLASS |
                         LIBUSB_RECIPIENT_INTERFACE |
                         LIBUSB_ENDPOINT_OUT;  // 0x21

  message.request = LIBUSB_REQUEST_SET_CONFIGURATION;
  message.value = (0x03 << 8) | hw_outlet;
  message.index = 0;
  message.data = buffer;
  message.data_size = sizeof(buffer);

  auto res = UsbSendCommand(&message);
  return !!res;
}


//-----------------------------------------------------------------------------
bool PowerCtl::GetOutletState(uint8_t outlet) const {
  uint8_t hw_outlet = 3 * outlet;
  uint8_t buffer[2] = { hw_outlet, 0x03 };
  UsbMessage message;

  printf("[Reading state:] 0x03 sent\n");

  message.request_type = LIBUSB_REQUEST_TYPE_CLASS |
                         LIBUSB_RECIPIENT_INTERFACE |
                         LIBUSB_ENDPOINT_IN;  // 0xa1

  message.request = 0x01;
  message.value = (0x03 << 8) | hw_outlet;
  message.index = 0;
  message.data = buffer;
  message.data_size = sizeof(buffer);

  UsbSendCommand(&message);

  printf("[Reading state:] 0x%02x recevied\n", buffer[1]);

  return buffer[1] & 1;
}


//-----------------------------------------------------------------------------
int PowerCtl::UsbSendCommand(UsbMessage* p_message) const {
  int res = ::libusb_control_transfer(p_hdev_,
                                      p_message->request_type,
                                      p_message->request,
                                      p_message->value,
                                      p_message->index,
                                      p_message->data,
                                      p_message->data_size,
                                      p_message->timeout);

  return res;
}


//-----------------------------------------------------------------------------
void PowerCtl::PrintDevice(libusb_device* p_dev) const {
  libusb_device_descriptor desc;

  int r = ::libusb_get_device_descriptor(p_dev, &desc);
  if (r < 0) {
    fprintf(stderr, "[!] Error getting device descriptor\n");
  }

  fprintf(stdout, "=====================\n");
  fprintf(stdout, "Nb of possible configurations: %i\n",
                  desc.bNumConfigurations);
  fprintf(stdout, "Device Class: %i\n", desc.bDeviceClass);
  fprintf(stdout, "VendorID: %04x\n", desc.idVendor);
  fprintf(stdout, "ProductID: %04x\n", desc.idProduct);
}
