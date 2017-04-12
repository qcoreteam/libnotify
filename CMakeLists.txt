cmake_minimum_required(VERSION 3.4.3 FATAL_ERROR)

project(libnotify CXX)

# Add path for custom CMake modules
list(APPEND CMAKE_MODULE_PATH
     "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

include(AddCompilerFlags)

set(NOTIFY_VERSION_MAJOR 0)
set(NOTIFY_VERSION_MINOR 0)
set(NOTIFY_VERSION_PATCH 1)
set(NOTIFY_VERSION_STAGE "alpha")

set(NOTIFY_ABI_MAJOR ${NOTIFY_VERSION_MAJOR})
set(NOTIFY_ABI_MINOR ${NOTIFY_VERSION_MINOR})
set(NOTIFY_ABI_PATCH ${NOTIFY_VERSION_PATCH})

set(NOTIFY_ABI_VERSION
    "${NOTIFY_ABI_MAJOR}.${NOTIFY_ABI_MINOR}.${NOTIFY_ABI_PATCH}")
set(NOTIFY_PACKAGE_VERSION
    "${NOTIFY_VERSION_MAJOR}.${NOTIFY_VERSION_MINOR}.${NOTIFY_VERSION_PATCH}")
set(NOTIFY_NUMERIC_VERSION 0x00000101)
set(NOTIFY_STAGE_NAME ${NOTIFY_VERSION_STAGE})

set(NOTIFY_ALLOWED_STAGE_NAMES
    rc
    beta
    alpha
    alpha-devel
    release
    stable)

list(FIND NOTIFY_ALLOWED_STAGE_NAMES
     "${NOTIFY_STAGE_NAME}"
     NOTIFY_TEMP_STAGE_RET)

if(NOTIFY_TEMP_STAGE_RET EQUAL -1)
   message(WARNING
           "stage ${NOTIFY_STAGE_NAME} is not allowed, reset to be beta")
   set(NOTIFY_STAGE_NAME beta)
endif()

set(NOTIFY_VERSION
    "${NOTIFY_VERSION_MAJOR}.${NOTIFY_VERSION_MINOR}.${NOTIFY_VERSION_PATCH}-${NOTIFY_STAGE_NAME}")

# Set some important variables
set(NOTIFY_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(NOTIFY_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}")
set(NOTUFY_CMAKE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")
set(NOTIFY_INCLUDE_DIR "${NOTIFY_SOURCE_DIR}/include")
set(NOTIFY_EXTRA_INCLUDE_DIR "${NOTIFY_BINARY_DIR}/include")
set(NOTIFY_RUNTIME_OUTPUT_INTDIR "${NOTIFY_BINARY_DIR}/${CMAKE_CFG_INTDIR}/bin")
set(NOTIFY_LIBRARY_OUTPUT_INTDIR "${NOTIFY_BINARY_DIR}/${CMAKE_CFG_INTDIR}/lib")
set(NOTIFY_ARCHIVE_OUTPUT_INTDIR "${NOTIFY_BINARY_DIR}/${CMAKE_CFG_INTDIR}/lib")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

option(NOTIFY_OPT_ENABLE_VERBOSE_DEBUG
       "Enable verbose debugging" OFF)

option(NOTIFY_OPT_DISABLE_TESTS
       "If tests should be compiled or not" OFF)

option(NOTIFY_OPT_ENABLE_COVERAGE
       "Enable running gcov to get a test coverage report (only works with GCC or Clang)
Make sure to enable -DCMAKE_BUILD_TYPE=Debug as well"
       OFF)

if(NOTIFY_OPT_ENABLE_VERBOSE_DEBUG)
   add_definitions(-DNOTIFY_DEBUG_MODE 1)
endif()

if(NOTIFY_OPT_ENABLE_COVERAGE)
   if(NOT "${CMAKE_BUILD_TYPE_LOWER}" STREQUAL "debug")
      message(FATAL_ERROR "Coverage requires -DCMAKE_BUILD_TYPE=Debug")
   endif()
   message(STATUS "Setting coverage compiler flags")
   set(CMAKE_REQUIRED_LIBRARIES "--converage")
   add_compiler_flags(-g -O0 --converage)
   set(CMAKE_REQUIRED_LIBRARIES "")
endif()

# GCC specific options.
if(CMAKE_COMPILER_IS_GNUCXX)
   option(NOTIFY_OPT_DISABLE_GCC_WARNINGS "Disable verbose warnings with GCC"
          OFF)
   option(NOTIFY_OPT_ENABLE_GCC_HARDENING "Enable compiler security checks"
          OFF)
   option(NOTIFY_OPT_ENABLE_GCC_FUNCTION_SECTIONS "Enable gcc function sections"
          OFF)
   option(NOTIFY_OPT_ENABLE_GCC_WARNINGS "Make all GCC warnings inti errors"
          OFF)
   
   list(APPEND NOTIFY_TEMP_FLAGS -Wall -Wswitch)
   if(NOTIFY_OPT_DISABLE_GCC_WARNINGS)
      list(APPEND NOTIFY_TEMP_FLAGS -w)
   endif()
   
   if(NOTIFY_OPT_ENABLE_GCC_HARDENING)
      list(APPEND NOTIFY_TEMP_FLAGS
           -fstack-protector-all
           -fwrapv
           -fPIE
           -Wstack-protector
           "--param ssp-buffer-size=1")
      add_definitions(-DNOTIFY_FORTIFY_SOURCE=2)
   endif()
   
   if(NOTIFY_OPT_ENABLE_GCC_FUNCTION_SECTIONS)
      list(APPEND NOTIFY_TEMP_FLAGS -ffunction-sections)
   endif()
   
   if(NOTIFY_OPT_ENABLE_GCC_WARNINGS)
      list(APPEND NOTIFY_TEMP_FLAGS -Werror)
   endif()
   
   # We need to test for at least gcc 2.95 here, because older versions don't
   # have -fno-strict-aliasing
   list(APPEND NOTIFY_TEMP_FLAGS -fno-strict-aliasing)
   notify_add_compiler_flags(${NOTIFY_TEMP_FLAGS})
endif()

# Get rid of deprecated warnings for OpenSSL on OSX 10.7 and greater.
if(APPLE)
   notify_add_compiler_flags(
       -Wno-error=deprecated-declarations
       -Qunused-arguments
   )
endif()

include_directories(BEFORE
                    ${NOTIFY_INCLUDE_DIR}
                    ${NOTIFY_EXTRA_INCLUDE_DIR})

if(NOTIFY_OPT_DISABLE_TESTS)
   add_subdirectory(tests)
   add_subdirectory(unittests)
endif()

add_subdirectory(cmake/modules)

message(STATUS "")
message(STATUS "(libnotify ${NOTIFY_VERSION})")
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")
message(STATUS "CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
message(STATUS "PROJECT_BINARY_DIR: ${PROJECT_BINARY_DIR}")
message(STATUS "PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")
message(STATUS "CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")
message(STATUS "CMAKE_COMMAND: ${CMAKE_COMMAND}")
message(STATUS "CMAKE_ROOT: ${CMAKE_ROOT}")
message(STATUS "CMAKE_SYSTEM: ${CMAKE_SYSTEM}")
message(STATUS "CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME}")
message(STATUS "CMAKE_SYSTEM_VERSION: ${CMAKE_SYSTEM_VERSION}")
message(STATUS "CMAKE_SYSTEM_PROCESSOR: ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "CMAKE_SKIP_RPATH: ${CMAKE_SKIP_RPATH}")
message(STATUS "CMAKE_VERBOSE_MAKEFILE: ${CMAKE_VERBOSE_MAKEFILE}")
message(STATUS "CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")
message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
message(STATUS "CMAKE_AR: ${CMAKE_AR}")
message(STATUS "CMAKE_RANLIB: ${CMAKE_RANLIB}")
message(STATUS "")