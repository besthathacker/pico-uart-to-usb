# This file is a helper to import the Pico SDK when it's not in a submodule
# It expects PICO_SDK_PATH to be set in the environment

if(NOT DEFINED PICO_SDK_PATH)
    message(FATAL_ERROR
        "PICO_SDK_PATH not set. Please set it to the path of the pico-sdk.")
endif()

set(PICO_SDK_INIT_CMAKE_FILE ${PICO_SDK_PATH}/external/pico_sdk_import.cmake)

if(NOT EXISTS ${PICO_SDK_INIT_CMAKE_FILE})
    message(FATAL_ERROR
        "Pico SDK not found at ${PICO_SDK_PATH}")
endif()

include(${PICO_SDK_INIT_CMAKE_FILE})
