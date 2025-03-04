cmake_minimum_required(VERSION 3.20.0)

set(BOARD stm32f030_demo)
set(DTO_DIR ${CMAKE_CURRENT_SOURCE_DIR}/boards)
set(DTC_OVERLAY_FILE ${DTO_DIR}/stm32f030_demo.overlay)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(led_blink)

target_sources(app PRIVATE src/main.c)