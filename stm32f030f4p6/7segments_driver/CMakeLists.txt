cmake_minimum_required(VERSION 3.20.0)

set(BOARD stm32f030_demo)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(7segments_driver)

target_sources(app PRIVATE src/main.c)