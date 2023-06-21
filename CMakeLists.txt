cmake_minimum_required(VERSION 3.12)
set(PICO_SDK_PATH $ENV{PICO_SDK_PATH})
set(PICO_SDK_INIT_CMAKE_FILE ${PICO_SDK_PATH}/pico_sdk_init.cmake)
include(${PICO_SDK_INIT_CMAKE_FILE})
project(rfid_rc522 LANGUAGES C CXX)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()
add_compile_options(-Wall -Wno-format -Werror -O3 -g0 -fdata-sections -ffunction-sections -Wl,--gc-sections)
include_directories(src/include)
include_directories(src/include/pico_usb_console/config)
add_executable(rfid_rc522
	src/main.c
	src/pico_usb_console/descriptor.c
	src/pico_usb_console/protocol.c
	src/pico_usb_console/serial.c
	src/rfid_rc522/rc522.c
)
pico_enable_stdio_usb(rfid_rc522 0)
pico_enable_stdio_uart(rfid_rc522 0)
target_link_libraries(rfid_rc522 pico_stdlib tinyusb_device tinyusb_board hardware_spi)
pico_add_extra_outputs(rfid_rc522)
