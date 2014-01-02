// Copyright (c) 2006-2008 The Elastic-Beam Authors. Beerware License.

#include "power_ctl.h"

#include "scope_exit.h"

#include <stdio.h>
#include <libusb.h>



PowerCtl::PowerCtl(const PowerStrip& product)
  : product_(product) { }



PowerCtl::~PowerCtl() {
  if (p_hdev_) {
    ::libusb_close(p_hdev_);
  }

  if (p_ctx_) {
    ::libusb_exit(p_ctx_);
  }
}


void PowerCtl::TurnOn(uint8_t outlet) {
  SetOutletState(outlet, kOn);
}



void PowerCtl::TurnOff(uint8_t outlet) {
  SetOutletState(outlet, kOff);
}





bool  PowerCtl::Init() {
  libusb_device_handle* p_hdev = nullptr;
  libusb_context* p_ctx = nullptr;
  int ret = 0;

  OnScopeExit cleanup([&]() {
      if (p_hdev) {
        ::libusb_close(p_hdev);
      }

      if (p_ctx) {
        ::libusb_exit(p_ctx);
      }
    });


  if ((ret = ::libusb_init(&p_ctx)) != 0) {
    fprintf(stderr, "[!] Cannot initialize usb context: %s\n", ::libusb_error_name(ret));
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
    fprintf(stderr, "[!] Cannot set usb configuration for device\n", ::libusb_error_name(ret));
    return false;
  };

  if ((ret = ::libusb_claim_interface(p_hdev, 0)) != 0) {
    fprintf(stderr, "[!] Cannot claim interface for device: %s\n", ::libusb_error_name(ret));
    return false;
  }

  if ((ret = ::libusb_set_interface_alt_setting(p_hdev, 0, 0)) != 0) {
    fprintf(stderr, "[!] Cannot set alternate interface for device: %s\n", ::libusb_error_name(ret));
    return false;
  }

  p_hdev_ = p_hdev;
  p_ctx_ = p_ctx;

  p_hdev = nullptr;
  p_ctx = nullptr;

  return true;
}


int PowerCtl::EnumDevices() {
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


void PowerCtl::PrintSerial() const {
  int req_type = 0xa1;
  int req = 0x01;
  uint8_t buffer[5] = {0, 0, 0, 0, 0};


  if (auto ret = (::libusb_control_transfer(p_hdev_,
                                req_type,
                                req,
                                (0x03 << 8) | 1,
                                0,
                                buffer,
                                5,
                                5000) < 2)) {
    fprintf(stderr,"[!] Libusb error string: %s\nTerminating\n", ::libusb_error_name(ret));
  }

  fprintf(stdout, "Product Serial : %02x:%02x:%02x:%02x:%02x\n",
          buffer[0],
          buffer[1],
          buffer[2],
          buffer[3],
          buffer[4]);
}



bool PowerCtl::SetOutletState(uint8_t outlet, OutletAction action) {
  int req_type = 0x21;
  int req = 0x09;
  uint8_t hw_outlet = 3 * outlet;

  uint8_t buffer[2] = {hw_outlet, action};
  auto res = ::libusb_control_transfer(p_hdev_,
                                       req_type,
                                       req,
                                       (0x03 << 8) | hw_outlet,
                                       0,
                                       buffer,
                                       2,
                                       500);
  printf("%i\n", res);
  return !!res;
}


bool PowerCtl::UsbSendCommand(uint8_t outlet, uint8_t request_type, uint8_t request) {
  uint8_t hw_outlet = 3 * outlet;

}


void PowerCtl::PrintDevice(libusb_device* p_dev) {
  libusb_device_descriptor desc;

  int r = ::libusb_get_device_descriptor(p_dev, &desc);
  if (r < 0) {
    fprintf(stderr, "[!] Error getting device descriptor\n");
  }

  fprintf(stdout, "=====================\n");
  fprintf(stdout, "Nb of possible configurations: %i\n", desc.bNumConfigurations);
  fprintf(stdout, "Device Class: %i\n", desc.bDeviceClass);
  fprintf(stdout, "VendorID: %04x\n", desc.idVendor);
  fprintf(stdout, "ProductID: %04x\n", desc.idProduct);
}
