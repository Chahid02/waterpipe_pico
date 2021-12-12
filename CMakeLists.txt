# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# initalize pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

set(PICO_SDK_PATH "/home/thomas/pico/pico-sdk")
#set(PICO_SDK_PATH "/home/luki/pico/pico-sdk")



# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(waterpipe C CXX ASM)
# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable( waterpipe 
                waterpipe.c 
                bme280.c 
                ds18b20.c 
                waterlevel.c 
                hc05.c
                aes.c)

pico_set_program_name(waterpipe "waterpipe")
pico_set_program_version(waterpipe "0.1")

pico_enable_stdio_uart(waterpipe 0)
pico_enable_stdio_usb(waterpipe 1)

# Add the standard library to the build
target_link_libraries(  waterpipe
                        pico_stdlib 
                        hardware_i2c 
                        pico_binary_info 
                        hardware_adc 
                        hardware_dma 
                        hardware_uart 
                        hardware_irq
                        pico_multicore)

target_sources(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/aes.c
    INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/aes.h
         ${CMAKE_CURRENT_LIST_DIR}/aes.hpp)



# Add any user requested libraries
target_link_libraries(waterpipe)

pico_add_extra_outputs(waterpipe)

