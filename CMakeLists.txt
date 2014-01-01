cmake_minimum_required(VERSION 2.8)

project(aqua_power_control)

list(APPEND CMAKE_CXX_FLAGS "-std=c++11")

set(THIRD_PARTY_DIR src/third_party)

#Libusb Related search
set(LIBUSB_INSTALL_DIR ${THIRD_PARTY_DIR}/libusb/libusb-1.0.9)
set(LIBUSB_1_INCLUDE_DIRS ${LIBUSB_INSTALL_DIR}/include/libusb-1.0/)

find_library(LIBUSB_1_LIBRARY
  NAMES usb-1.0
  PATHS ${LIBUSB_INSTALL_DIR}
  PATH_SUFFIXES lib
  NO_DEFAULT_PATH
)

message(${LIBUSB_INSTALL_DIR})
message(${LIBUSB_1_LIBRARY})

#set(LIBUSB_1_LIBRARIES ${LIBUSB_1_LIBRARY})


include_directories(./src/ ${LIBUSB_1_INCLUDE_DIRS})

set(SOURCE_FILES
  src/main.cc
  src/power_ctl.cc
  src/power_ctl.h)

add_executable(aqua_power_ctl
  ${SOURCE_FILES})


target_link_libraries(aqua_power_ctl ${LIBUSB_1_LIBRARY})

install(TARGETS aqua_power_ctl DESTINATION bin)

