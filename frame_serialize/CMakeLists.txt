ADD_LIBRARY(FrameSerialize STATIC
        JFJochFrameSerializer.cpp JFJochFrameSerializer.h
        JFJochFrameDeserializer.cpp JFJochFrameDeserializer.h
        tinycbor/src/cborencoder.c
        tinycbor/src/cborencoder_close_container_checked.c
        tinycbor/src/cborencoder_float.c
        tinycbor/src/cborparser.c
        tinycbor/src/cborparser_float.c
        tinycbor/src/cborpretty.c
        tinycbor/src/cborerrorstrings.c
        tinycbor/src/cbor.h
        tinycbor/src/tinycbor-version.h CborErr.h)
