cmake_minimum_required(VERSION 3.20.0)

set(BOARD esp32_devkitc_wroom)
set(DTO_DIR ${CMAKE_CURRENT_SOURCE_DIR}/boards)
set(DTC_OVERLAY_FILE ${DTO_DIR}/esp32_devkit_wroom.overlay)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})


project(signal_capture)

target_sources(app PRIVATE src/main.c)