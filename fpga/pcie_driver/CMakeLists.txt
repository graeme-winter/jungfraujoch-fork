# Kernel module should not be compiled with CMake
# but it helps in code completion in CLion IDE

ADD_LIBRARY(jfjoch_kernel_module STATIC
        ioctl.h jfjoch_dev.c jfjoch_dev.h ioctl.h)

EXECUTE_PROCESS(COMMAND uname -r
        OUTPUT_VARIABLE KERNEL_RELEASE
        OUTPUT_STRIP_TRAILING_WHITESPACE)


FIND_PATH(KERNELHEADERS_DIR
        include/linux/user.h
        PATHS /usr/src/linux-headers-${KERNEL_RELEASE}
        )

MESSAGE(STATUS "Kernel release: ${KERNEL_RELEASE}")

IF(KERNELHEADERS_DIR)
    MESSAGE(STATUS "Kernel headers: ${KERNELHEADERS_DIR}")
    TARGET_INCLUDE_DIRECTORIES(jfjoch_kernel_module PRIVATE
            ${KERNELHEADERS_DIR}/include
            ${KERNELHEADERS_DIR}/arch/x86/include
            ${KERNELHEADERS_DIR}/include/generated)
ENDIF()

SET_TARGET_PROPERTIES(jfjoch_kernel_module PROPERTIES EXCLUDE_FROM_ALL 1)

TARGET_COMPILE_DEFINITIONS(jfjoch_kernel_module PRIVATE -D__KERNEL__ -DMODULE -DKBUILD_MODNAME=\"jfjoch\")
