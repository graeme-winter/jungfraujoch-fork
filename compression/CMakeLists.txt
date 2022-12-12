ADD_LIBRARY(Compression STATIC
        lz4/lz4.c
        lz4/lz4.h
        bitshuffle/bitshuffle.c
        bitshuffle/bitshuffle_core.c
        bitshuffle/iochain.c
        JFJochZstdCompressor.cpp
        JFJochZstdCompressor.h
        JFJochCompressor.cpp
        JFJochCompressor.h
        JFJochDecompress.h
        MaxCompressedSize.cpp
        MaxCompressedSize.h)

TARGET_COMPILE_DEFINITIONS(Compression PUBLIC -DZSTD_SUPPORT -DUSE_ZSTD)
TARGET_LINK_LIBRARIES(Compression libzstd_static)
TARGET_INCLUDE_DIRECTORIES(Compression PUBLIC . zstd/lib)
ADD_SUBDIRECTORY(zstd/build/cmake)

if (${CMAKE_HOST_SYSTEM_PROCESSOR} MATCHES "ppc64le")
    TARGET_COMPILE_DEFINITIONS(Compression PUBLIC -DUSESSE2 -DNO_WARN_X86_INTRINSICS)
endif()