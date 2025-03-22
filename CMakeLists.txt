# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")
set(PICO_LWIP 1)

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)
project(project_main C CXX ASM)

file(GLOB SOURCES "src/*.c")
# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(project_main ${SOURCES} )

pico_set_program_name(project_main "project_main")
pico_set_program_version(project_main "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(project_main 0)
pico_enable_stdio_usb(project_main 1)

add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/lib/pico-ssd1306")


# Add the standard library to the build
target_link_libraries(project_main
    pico_stdlib
    pico_cyw43_arch_lwip_threadsafe_background
    pico_lwip
    pico-ssd1306
    hardware_pwm
    )

# Add the standard include files to the build
target_include_directories(project_main PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/lib/pico-ssd1306
    ${CMAKE_SOURCE_DIR}/inc
)

target_compile_definitions(project_main PRIVATE
    PICO_LWIP_IPV4_ENABLED
    PICO_LWIP_TCP_ENABLED
    PICO_LWIP_UDP_ENABLED
    MEM_SIZE=8000
    MEMP_NUM_TCP_PCB=10
    MEMP_NUM_UDP_PCB=10
    MEMP_NUM_TCP_SEG=64
    MEMP_NUM_PBUF=64
    WiFI_SSID=${WIFI_SSID}
    WiFI_PASSWORD=${WiFI_PASSWORD}
)

pico_add_extra_outputs(project_main)