cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(pico_usb_uart C CXX ASM)
pico_sdk_init()

add_executable(pico_usb_uart
    main.c
)

target_include_directories(pico_usb_uart PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}  # so tusb_config.h is found
)

target_link_libraries(pico_usb_uart
    pico_stdlib
    hardware_uart
    tinyusb_device
    tinyusb_board
)

# disable default stdio (we handle USB CDC manually)
pico_enable_stdio_usb(pico_usb_uart 0)
pico_enable_stdio_uart(pico_usb_uart 0)

pico_add_extra_outputs(pico_usb_uart)
