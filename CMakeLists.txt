cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# When this file is the top-level project (built standalone), set up the
# Pico SDK and a test executable. When included via add_subdirectory() by
# a parent project, skip this block entirely — the parent already owns
# the SDK setup and project() declaration.
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    # == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
    if(WIN32)
        set(USERHOME $ENV{USERPROFILE})
    else()
        set(USERHOME $ENV{HOME})
    endif()
    set(sdkVersion 2.2.0)
    set(toolchainVersion 14_2_Rel1)
    set(picotoolVersion 2.2.0)
    set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
    if (EXISTS ${picoVscode})
        include(${picoVscode})
    endif()
    # ====================================================================================
    set(PICO_BOARD pico2 CACHE STRING "Board type")

    include(pico_sdk_import.cmake)
    project(pico_canlib C CXX ASM)
    pico_sdk_init()
endif()

# ---------------------------------------------------------------------------
# Library target — always defined, whether standalone or subdirectory.
# Any project that calls add_subdirectory(lib/pico_canlib) gets this target.
# ---------------------------------------------------------------------------
add_library(pico_canlib STATIC
    Src/pico_canlib.cpp
)

# PUBLIC: any target that links pico_canlib inherits this include path,
# so callers never need to manually add Inc/ to their own includes.
target_include_directories(pico_canlib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Inc/
)

target_link_libraries(pico_canlib PUBLIC
    pico_stdlib
    hardware_spi
    hardware_i2c
    hardware_dma
    hardware_pio
    hardware_interp
    hardware_timer
    hardware_watchdog
    hardware_clocks
)

# ---------------------------------------------------------------------------
# Standalone-only: unit test executable.
# Not built when this is a subdirectory of another project.
# ---------------------------------------------------------------------------
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    add_executable(pico_canlib_test
        Src/unit_test.cpp
    )

    pico_set_program_name(pico_canlib_test "pico_canlib_test")
    pico_set_program_version(pico_canlib_test "0.1")

    pico_enable_stdio_uart(pico_canlib_test 1)
    pico_enable_stdio_usb(pico_canlib_test 1)

    target_link_libraries(pico_canlib_test PRIVATE pico_canlib)

    pico_add_extra_outputs(pico_canlib_test)
endif()
