INCLUDE(CMakeForceCompiler)

SET(CMAKE_SYSTEM_NAME Linux) # this one is important
SET(CMAKE_SYSTEM_VERSION 1)  # this one not so much

SET(RPI_ROOT /opt/cross/x-tools/arm-unknown-linux-gnueabi)

SET(CMAKE_C_COMPILER   ${RPI_ROOT}/bin/arm-unknown-linux-gnueabi-gcc)
SET(CMAKE_CXX_COMPILER ${RPI_ROOT}/bin/arm-unknown-linux-gnueabi-c++)
SET(CMAKE_AR           ${RPI_ROOT}/bin/arm-unknown-linux-gnueabi-ar)
SET(CMAKE_LINKER       ${RPI_ROOT}/bin/arm-unknown-linux-gnueabi-ld)
SET(CMAKE_NM           ${RPI_ROOT}/bin/arm-unknown-linux-gnueabi-nm)
SET(CMAKE_OBJCOPY      ${RPI_ROOT}/bin/arm-unknown-linux-gnueabi-objcopy)
SET(CMAKE_OBJDUMP      ${RPI_ROOT}/bin/arm-unknown-linux-gnueabi-objdump)
SET(CMAKE_STRIP        ${RPI_ROOT}/bin/arm-unknown-linux-gnueabi-strip)
SET(CMAKE_RANLIB       ${RPI_ROOT}/bin/arm-unknown-linux-gnueabi-tanlib)

# where is the target environment
#SET(CMAKE_FIND_ROOT_PATH  ${RPI_ROOT}/arm-unknown-linux-gnueabi)

# search for programs in the build host directories
#SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
