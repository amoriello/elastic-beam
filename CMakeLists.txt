cmake_minimum_required(VERSION 2.8)

project(aqua_power_control)

list(APPEND CMAKE_CXX_FLAGS "-std=c++11")

set(THIRD_PARTY_DIR ${PROJECT_SOURCE_DIR}/src/third_party)

#Libusb Related search
if (UNIX)
	message("Configuring for Linux")
	set(LIBUSB_INSTALL_DIR ${THIRD_PARTY_DIR}/libusb/libusb-1.0.9)
	set(LIBUSB_1_INCLUDE_DIRS ${LIBUSB_INSTALL_DIR}/include/libusb-1.0/)
	set(LIBUSB_1_SEARCH_PATH ${LIBUSB_INSTALL_DIR}/lib/)
endif(UNIX)

if (MSVC)
	message("Configuring for Windows")
	set(LIBUSB_INSTALL_DIR ${THIRD_PARTY_DIR}/libusb/libusb-1.0.9)
	set(LIBUSB_1_INCLUDE_DIRS ${LIBUSB_INSTALL_DIR}/libusb)
	set(LIBUSB_1_SEARCH_PATH ${LIBUSB_INSTALL_DIR}/Win32/Debug/lib/)
endif(MSVC)

find_library(LIBUSB_1_LIBRARY
  NAMES usb-1.0
  PATHS ${LIBUSB_1_SEARCH_PATH}
  NO_DEFAULT_PATH
)

message(${LIBUSB_INSTALL_DIR})
message(${LIBUSB_1_LIBRARY})


include_directories(./src/ ${LIBUSB_1_INCLUDE_DIRS})

set(SOURCE_FILES
  src/main.cc
  src/power_ctl.cc
  src/power_ctl.h)

add_executable(aqua_power_ctl
  ${SOURCE_FILES})

if (UNIX)
  target_link_libraries(aqua_power_ctl ${LIBUSB_1_LIBRARY})
endif (UNIX)

if (MSVC)
  # This is temporary (allowing edition of source code from vs12),
  # Need to do a better FindLibusb script
  # Todo (Amoriello) : FindLibusb
  target_link_libraries(aqua_power_ctl)
endif(MSVC)

install(TARGETS aqua_power_ctl DESTINATION bin)

