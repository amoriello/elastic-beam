cmake_minimum_required(VERSION 2.8)
project(power_controller)

# Local project third_party directory
set(THIS_THIRD_PARTY_DIR "${PROJECT_SOURCE_DIR}/third_party")

# Boost related search
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost 1.55.0 REQUIRED COMPONENTS program_options)



# Libusb Related search
if (UNIX)
	message("Configuring for Linux")
	list(APPEND CMAKE_CXX_FLAGS "-std=c++11")
	set(LIBUSB_INSTALL_DIR "${THIS_THIRD_PARTY_DIR}/libusb/libusb-1.0.9")
	set(LIBUSB_1_INCLUDE_DIRS "${LIBUSB_INSTALL_DIR}/include/libusb-1.0")
	set(LIBUSB_1_SEARCH_PATH "${LIBUSB_INSTALL_DIR}/lib")
endif(UNIX)

if (MSVC)
	message("Configuring for Windows")
	set(LIBUSB_INSTALL_DIR "${THIS_THIRD_PARTY_DIR}/libusb/libusb-1.0.9")
	set(LIBUSB_1_INCLUDE_DIRS "${LIBUSB_INSTALL_DIR}/libusb")
	set(LIBUSB_1_SEARCH_PATH "${LIBUSB_INSTALL_DIR}/Win32/Debug/lib")
endif(MSVC)

find_library(LIBUSB_1_LIBRARY
  NAMES usb-1.0
  PATHS ${LIBUSB_1_SEARCH_PATH}
  NO_DEFAULT_PATH
)


include_directories(${LIBUSB_1_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

set(SOURCE_FILES
  main.cc
  power_ctl.cc
  power_ctl.h
  program_options.cc
  program_options.h
  products.h
  scope_exit.h)

add_executable(power_controller ${SOURCE_FILES})

if (UNIX)
  target_link_libraries(power_controller ${LIBUSB_1_LIBRARY} ${Boost_LIBRARIES})
endif (UNIX)

if (MSVC)
  # This is temporary (allowing edition of source code from vs12),
  # Need to do a better FindLibusb script
  # Todo (Amoriello) : FindLibusb
  target_link_libraries(power_controller)
endif(MSVC)

