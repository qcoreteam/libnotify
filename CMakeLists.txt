cmake_minimum_required(VERSION 3.4.3 FATAL_ERROR)

project(libnotify CXX)

# Add path for custom CMake modules
list(APPEND CMAKE_MODULE_PATH
     "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

include(AddCompilerFlags)
include(CheckCXXSymbolExists)
include(CheckIncludeFileCXX)
include(CheckFunctionExists)

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

option(NOTIFY_OPT_DISABLE_CLOCK_GETTIME
       "Do not use clock_gettime event if it is available"
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

if(NOT NOTIFY_GNU_SOURCE)
   check_cxx_symbol_exists(__GNU_LIBRARY__ "features.h" NOTIFY_GNU_SOURCE)
   if(NOT NOTIFY_GNU_SOURCE)
      unset(NOTIFY_GNU_SOURCE CACHE)
      check_cxx_symbol_exists(_GNU_SOURCE "features.h" NOTIFY_GNU_SOURCE)
   endif()
endif()

if(NOT NOTIFY_GNU_SOURCE)
   add_definitions(-DNOTIFY_GNU_SOURCE)
endif()

check_include_file_cxx(sys/types.h NOTIFY_HAVE_SYS_TYPES_H)
if(NOTIFY_HAVE_SYS_TYPES_H)
   list(APPEND CMAKE_EXTRA_INCLUDE_FILES sys/types.h)
endif()

check_include_file_cxx(sys/socket.h NOTIFY_HAVE_SYS_SOCKET_H)
if(NOTIFY_HAVE_SYS_SOCKET_H)
   list(APPEND CMAKE_EXTRA_INCLUDE_FILES sys/socket.h)
endif()

check_include_file_cxx(netinet/in.h NOTIFY_HAVE_NETINET_IN_H)
if(NOTIFY_HAVE_NETINET_IN_H)
   list(APPEND CMAKE_EXTRA_INCLUDE_FILES netinet/in.h)
endif()

check_include_file_cxx(netinet/in6.h NOTIFY_HAVE_NETINET_IN6_H)
if(NOTIFY_HAVE_NETINET_IN6_H)
   list(APPEND CMAKE_EXTRA_INCLUDE_FILES netinet/in6.h)
endif()

check_include_file_cxx(unistd.h NOTIFY_HAVE_UNISTD_H)
check_include_file_cxx(netdb.h NOTIFY_HAVE_NETDB_H)
check_include_file_cxx(dlfcn.h NOTIFY_HAVE_DLFCN_H)
check_include_file_cxx(arpa/inet.h NOTIFY_HAVE_ARPA_INET_H)
check_include_file_cxx(fcntl.h NOTIFY_HAVE_FCNTL_H)
if(NOTIFY_HAVE_FCNTL_H)
   list(APPEND CMAKE_EXTRA_INCLUDE_FILES fcntl.h)
endif()
check_include_file_cxx(inttypes.h NOTIFY_HAVE_INTTYPES_H)
check_include_file_cxx(memory.h NOTIFY_HAVE_MEMORY_H)
check_include_file_cxx(poll.h NOTIFY_HAVE_POLL_H)
check_include_file_cxx(port.h NOTIFY_HAVE_PORT_H)
if(NOTIFY_HAVE_PORT_H)
   list(APPEND CMAKE_EXTRA_INCLUDE_FILES port.h)
endif()
check_include_file_cxx(signal.h NOTIFY_HAVE_SIGNAL_H)
check_include_file_cxx(stdarg.h NOTIFY_HAVE_STDARG_H)
check_include_file_cxx(stddef.h NOTIFY_HAVE_STDDEF_H)
check_include_file_cxx(stdint.h NOTIFY_HAVE_STDINT_H)
check_include_file_cxx(stdlib.h NOTIFY_HAVE_STDLIB_H)
check_include_file_cxx(strings.h NOTIFY_HAVE_STRINGS_H)
check_include_file_cxx(string.h NOTIFY_HAVE_STRING_H)
check_include_file_cxx(sys/devpoll.h NOTIFY_HAVE_SYS_DEV_POLL_H)
check_include_file_cxx(sys/epoll.h NOTIFY_HAVE_SYS_EPOLL_H)
check_include_file_cxx(sys/eventfd.h NOTIFY_HAVE_SYS_EVENTFD_H)
check_include_file_cxx(sys/event.h NOTIFY_HAVE_SYS_EVENT_H)
check_include_file_cxx(sys/ioctl.h NOTIFY_HAVE_SYS_IOCTL_H)
check_include_file_cxx(sys/mman.h NOTIFY_HAVE_SYS_MMAN_H)
check_include_file_cxx(sys/param.h NOTIFY_HAVE_SYS_PARAM_H)
check_include_file_cxx(sys/queue.h NOTIFY_HAVE_SYS_QUEUE_H)
check_include_file_cxx(sys/select.h NOTIFY_HAVE_SYS_SELECT_H)
check_include_file_cxx(sys/sendfile.h NOTIFY_HAVE_SYS_SENDFILE_H)
check_include_file_cxx(sys/stat.h NOTIFY_HAVE_SYS_STAT_H)
check_include_file_cxx(sys/time.h NOTIFY_HAVE_SYS_TIME_H)
if(NOTIFY_HAVE_SYS_TIME_H)
   list(APPEND CMAKE_EXTRA_INCLUDE_FILESsys/time.h)
endif()
check_include_file_cxx(sys/uio.h NOTIFY_HAVE_SYS_UIO_H)
check_include_file_cxx(sys/types.h NOTIFY_HAVE_SYS_TYPES_H)
check_include_file_cxx(ifaddrs.h NOTIFY_HAVE_SYS_IFADDRS_H)
check_include_file_cxx(mach/mach_time.h NOTIFY_HAVE_MACH_MACH_TIME_H)
check_include_file_cxx(netinet/tcp.h NOTIFY_HAVE_NETINET_TCP_H)
check_include_file_cxx(sys/wait.h NOTIFY_HAVE_WAIT_H)
check_include_file_cxx(sys/resource.h NOTIFY_SYS_RESOURCE_H)
check_include_file_cxx(sys/sysctl.h NOTIFY_SYS_SYSCTL_H)
check_include_file_cxx(sys/timerfd.h NOTIFY_SYS_TIMERFD_H)
check_include_file_cxx(errno.h NOTIFY_HAVE_ERRNO_H)

check_function_exists(epoll_create NOTIFY_HAVE_EPOLL)
check_function_exists(epoll_ctl NOTIFY_HAVE_EPOLL_CTL)
check_function_exists(eventfd NOTIFY_HAVE_EVENTFD)
if(NOT NOTIFY_DISABLE_CLOCK_GETTIME)
   check_function_exists(clock_gettime NOTIFY_HAVE_CLOCK_GETTIME)
endif()
check_function_exists(fcntl NOTIFY_HAVE_FCNTL)
check_function_exists(getaddrinfo NOTIFY_HAVE_GETADDRINFO)
check_function_exists(getnameinfo NOTIFY_HAVE_GETNAMEINFO)
check_function_exixts(gettimeofday NOTIFY_HAVE_GETTIMEOFDAY)
check_function_exists(getprotobynumber NOTIFY_HAVE_GETPROTOBYNUMBER)

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
