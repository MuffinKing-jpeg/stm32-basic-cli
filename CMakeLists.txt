cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME DEBUGING_WITH_F303)

# Include toolchain file

include("cmake/gcc-arm-none-eabi.cmake")


# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)
add_subdirectory(Drivers/AHT20)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)
# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    stm32cubemx
    aht20
    # Add user defined libraries
)
file(GLOB SRC_FILES "./Core/Src/cli/*.c")
file(GLOB DBG_FILES "./Core/Src/debug/*.c")
# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # ${CMAKE_SOURCE_DIR}/Core/Src/cli.c
    # ${CMAKE_SOURCE_DIR}/Core/Src/cli/cli.c
    ${SRC_FILES}
    # Add user sources here
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
${CMAKE_SOURCE_DIR}/Core/Inc/cli/
    # Add user defined include paths
)

target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

if( CMAKE_BUILD_TYPE STREQUAL "Debug" OR
    CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo"
    )
    add_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE BUILD_DEBUG)
    target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/Core/Inc/debug/
        
    )
    message("Debug is enabled")
    target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    ${DBG_FILES}
    )
else()
    message("Release")
    add_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE BUILD_RELEASE)
endif()

# Add project symbols (macros)



