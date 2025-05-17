cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(rpi_can LANGUAGES C CXX VERSION 1.0)

include(CheckFunctionExists)
include(CheckSymbolExists)
include(GNUInstallDirs)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# Collect source files
file(GLOB rpi_can_SRC
     "../Source/*.c"
     "../Source/*.cpp"
     "../Source/MyLib/*.c"
     "../Source/MyLib/*.cpp"
)

# Collect header files
file(GLOB rpi_can_HDR
     "../Source/"
     "../Source/MyLib"
)

# Add the executable
add_executable(rpi_can ${rpi_can_SRC})

# Add the include directories
target_include_directories(rpi_can PRIVATE ${rpi_can_HDR})

# Link the pthread library
target_link_libraries(rpi_can PRIVATE pthread)


# Add an option to enable treating warnings as errors
option(ENABLE_WERROR "Treat all compiler warnings as errors" ON)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

if(ENABLE_WERROR)
  target_compile_options(rpi_can PRIVATE -Werror)
endif()

# Add compiler definitions
target_compile_definitions(rpi_can PRIVATE
  _GNU_SOURCE
  SO_RXQ_OVFL=40
  PF_CAN=29
  AF_CAN=PF_CAN
  N_SLCAN=17
  SCM_TIMESTAMPING_OPT_STATS=54
  CLOCK_TAI=11
  SO_TXTIME=61
  SCM_TXTIME=SO_TXTIME
)

# Add compiler flags
target_compile_options(rpi_can PRIVATE
  -Wall
  -Wno-parentheses
  -Wsign-compare
  -fno-strict-aliasing
)

# Set default build type to Release if not specified
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Enable compile commands export
if(CMAKE_EXPORT_COMPILE_COMMANDS STREQUAL "")
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "project default" FORCE)
endif()

# Set the output directory for executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/rpi_can/build)

# Set the output directory for shared libraries
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/rpi_can/build)

# Set the output directory for static libraries
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/rpi_can/build)


# Print CMake version
message(STATUS "CMake version: ${CMAKE_VERSION}")