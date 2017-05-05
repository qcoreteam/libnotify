cmake_minimum_required(VERSION 3.4.3 FATAL_ERROR)

project(notify CXX)

# Add path for custom CMake modules
list(APPEND CMAKE_MODULE_PATH
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

include(AddCompilerFlags)
include(AddLibNotify)
include(CheckCXXSymbolExists)
include(CheckIncludeFileCXX)
include(CheckFunctionExists)
include(CheckLibraryExists)
include(DetermineGCCCompatible)

set(NOTIFY_VERSION_MAJOR 1)
set(NOTIFY_VERSION_MINOR 0)
set(NOTIFY_VERSION_PATCH 0)
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
set(NOTIFY_WORKSPACE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(NOTIFY_UTILS_DIR "${NOTIFY_WORKSPACE_DIR}/utils")
set(NOTIFY_SOURCE_DIR "${NOTIFY_WORKSPACE_DIR}/src")
set(NOTIFY_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}")
set(NOTUFY_CMAKE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")
set(NOTIFY_INCLUDE_DIR "${NOTIFY_WORKSPACE_DIR}/include")

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

option(NOTIFY_OPT_ENABLE_THREADS "Use threads if available." ON)

# Provide public options to globally control RTTI and EH
option(NOTIFY_OPT_DISABLE_EXCEPTION "Disable Exception handling" OFF)
option(NOTIFY_OPT_DISABLE_RTTI "Disable runtime type information" OFF)

option(NOTIFY_OPT_FORCE_KQUEUE_CHECK
        "When crosscompiling forces running a test program that verifies that Kqueue works with pipes. Note that this requires you to manually run the test program on the the cross compilation target to verify that it works. See cmake documentation for try_run for more details" OFF)

if(NOTIFY_OPT_DISABLE_EXCEPTION AND NOT NOTIFY_OPT_DISABLE_RTTI)
   message(FATAL_ERROR "Exception handling requires RTTI. You must set NOTIFY_OPT_DISABLE_EXCEPTION to OFF")
endif()

set(NOTIFY_OPT_LIBDIR_SUFFIX "" CACHE STRING "Define suffix of library directory name (32/64)" )

string(TOUPPER "${CMAKE_BUILD_TYPE}" NOTIFY_BUILD_TYPE)

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

if (CMAKE_VERSION VERSION_LESS "3.1")
   if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
      set(CMAKE_CXX_FLAGS "--std=gnu++14 ${CMAKE_CXX_FLAGS}")
   elseif(CMAKE_CXX_FLAGS STREQUAL "Clang")
      set(CMAKE_CXX_FLAGS "--std=c++14 ${CMAKE_CXX_FLAGS}")
   endif ()
else ()
   set(CMAKE_CXX_STANDARD 14)
endif ()

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
check_include_file_cxx(endian.h NOTIFY_HAVE_ENDIAN_H)
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
check_include_file_cxx(pthread.h NOTIFY_HAVE_PTHREAD_H)
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

# libraray checks
check_library_exists(pthread pthread_create "" NOTIFY_HAVE_PTHREAD)
if(NOTIFY_HAVE_PTHREAD)
   check_library_exists(pthread pthread_getspecific "" NOTIFY_HAVE_PTHREAD_GETSPECIFIC)
   check_library_exists(pthread pthread_rwlock_init "" NOTIFY_HAVE_PTHREAD_RWLOCK_INIT)
   check_library_exists(pthread pthread_mutex_lock "" NOTIFY_HAVE_PTHREAD_MUTEX_LOCK)
endif()
check_library_exists(dl dlopen "" NOTIFY_HAVE_DL)
check_library_exists(rt clock_gettime "" NOTIFY_HAVE_RT)

if(NOTIFY_HAVE_PTHREAD)
   # We want to find pthreads library and at the moment we do want to
   # have it reported as '-l<lib>' instead of '-pthread'.
   # TODO: switch to -pthread once the rest of the build system can deal with it.
   set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
   set(THREADS_HAVE_PTHREAD_ARG Off)
   find_package(Threads REQUIRED)
   set(PTHREAD_LIB ${CMAKE_THREAD_LIBS_INIT})
endif()

check_function_exists(epoll_create NOTIFY_HAVE_EPOLL)
check_function_exists(epoll_ctl NOTIFY_HAVE_EPOLL_CTL)
check_function_exists(eventfd NOTIFY_HAVE_EVENTFD)
if(NOT NOTIFY_DISABLE_CLOCK_GETTIME)
   check_function_exists(clock_gettime NOTIFY_HAVE_CLOCK_GETTIME)
endif()
check_function_exists(fcntl NOTIFY_HAVE_FCNTL)
check_function_exists(getaddrinfo NOTIFY_HAVE_GETADDRINFO)
check_function_exists(getnameinfo NOTIFY_HAVE_GETNAMEINFO)
check_function_exists(gettimeofday NOTIFY_HAVE_GETTIMEOFDAY)
check_function_exists(getprotobynumber NOTIFY_HAVE_GETPROTOBYNUMBER)
check_function_exists(getservbyname NOTIFY_HAVE_GETSERVERBYNAME)
check_function_exists(inet_ntop NOTIFY_HAVE_NTOP)
check_function_exists(inet_pton NOTIFY_HAVE_PTON)
check_function_exists(kqueue NOTIFY_HAVE_KQUEUE)
check_function_exists(mmap NOTIFY_HAVE_MMAP)
check_function_exists(pipe NOTIFY_HAVE_PIPE)
check_function_exists(pipe2 NOTIFY_HAVE_PIPE2)
check_function_exists(poll NOTIFY_HAVE_POLL)
check_function_exists(port_create NOTIFY_HAVE_PORT_CREATE)
check_function_exists(sendfile NOTIFY_HAVE_SENDFILE)
check_function_exists(sigaction NOTIFY_HAVE_SIGACTION)
check_function_exists(signal NOTIFY_HAVE_SIGNAL)
check_function_exists(splice NOTIFY_HAVE_SPLICE)
check_function_exists(strlcpy NOTIFY_HAVE_STRLCPY)
check_function_exists(strsep NOTIFY_HAVE_STRSEP)
check_function_exists(strtok_r NOTIFY_HAVE_STRTOK_R)
check_function_exists(strtoll NOTIFY_HAVE_STRTOLL)
check_function_exists(vasprintf NOTIFY_HAVE_VASPRINTF)
check_function_exists(sysctl NOTIFY_HAVE_SYSCTL)
check_function_exists(accept4 NOTIFY_HAVE_ACCEPT4)
check_function_exists(epoll_create1 NOTOFY_HAVE_EPOLL_CREATE1)
check_function_exists(getegid NOTIFY_HAVE_GETEGID)
check_function_exists(geteuid NOTIFY_HAVE_GETEUID)
check_function_exists(getifaddrs NOTIFY_HAVE_GETIFADDRS)
check_function_exists(issetugid NOTIFY_HAVE_ISSETUGID)
check_function_exists(mach_absolute_time NOTIFY_HAVE_MACH_ABSOLUTE_TIME)
check_function_exists(nanosleep NOTIFY_HAVE_NANOSLEEP)
check_function_exists(usleep NOTIFY_HAVE_USLEEP)
check_function_exists(timeradd NOTIFY_HAVE_TIMERADD)
check_function_exists(timerclear NOTIFY_HAVE_TIMERCLEAR)
check_function_exists(timercmp NOTIFY_HAVE_TIMERCMP)
check_function_exists(timerfd_create NOTIFY_HAVE_TIMERFD_CREATE)
check_function_exists(timerisset NOTIFY_HAVE_TIMERISSET)
check_function_exists(putenv NOTIFY_HAVE_PUTENV)
check_function_exists(getenv NOTIFY_HAVE_GETENV)
check_function_exists(setrlimit NOTIFY_HAVE_SETRLIMIT)
check_function_exists(umask NOTIFY_HAVE_UMASK)
check_function_exists(unsetenv NOTIFY_HAVE_UNSETENV)
check_function_exists(gethostbyname_r NOTIFY_HAVE_GETHOSTBYNAME_R)

if(NOTIFY_HAVE_PORT_H AND NOTIFY_HAVE_PORT_CREATE)
   set(NOTIFY_HAVE_EVENT_PORTS 1)
endif()

if(NOT WIN32)
   check_function_exists(select NOTIFY_HAVE_SELECT)
endif()

# check symbol
check_symbol_exists(_POSIX_1_SOURCE "stdio.h" NOTIFY_POSIX_1_SOURCE)
check_symbol_exists(_POSIX_SOURCE "stdio.h" NOTIFY_POSIX_SOURCE)

# Verify kqueue works with pipes.
if (NOTIFY_HAVE_KQUEUE)
   if (CMAKE_CROSSCOMPILING AND NOT NOTIFY_FORCE_KQUEUE_CHECK)
      message(WARNING "Cannot check if kqueue works with pipes when crosscompiling, use NOTIFY_FORCE_KQUEUE_CHECK to be sure (this requires manually running a test program on the cross compilation target)")
      set(NOTIFY_HAVE_WORKING_KQUEUE 1)
   else()
      message(STATUS "Checking if kqueue works with pipes...")
      include(CheckWorkingKqueue)
   endif()
endif()

if(NOT APPLE)
   execute_process(
           COMMAND ${CMAKE_C_COMPILER} -Wl,--version
           OUTPUT_VARIABLE stdout
           ERROR_QUIET
   )
   if("${stdout}" MATCHES "GNU gold")
      set(NOTIFY_LINKER_IS_GOLD ON)
   endif()
endif()

configure_file(
        ${NOTIFY_INCLUDE_DIR}/notify/kernel/PrivateConfig.h.in
        ${NOTIFY_EXTRA_INCLUDE_DIR}/notify/kernel/PrivateConfig.h
        NEWLINE_STYLE UNIX)

configure_file(
        ${NOTIFY_INCLUDE_DIR}/notify/kernel/Config.h.in
        ${NOTIFY_EXTRA_INCLUDE_DIR}/notify/kernel/Config.h
        NEWLINE_STYLE UNIX)

# generate config header

# We use *.dylib rather than *.so on darwin.
set(NOTIFY_PLUGIN_EXT ${CMAKE_SHARED_LIBRARY_SUFFIX})

include_directories(BEFORE
        ${NOTIFY_INCLUDE_DIR}
        ${NOTIFY_EXTRA_INCLUDE_DIR})

if(NOT NOTIFY_OPT_DISABLE_TESTS)
   add_subdirectory(utils/unittest)
   add_subdirectory(tests)
   add_subdirectory(unittests)
endif()

set(NOTIFY_HEADERS)
set(NOTIFY_PRIVATE_HEADERS)
add_subdirectory(cmake/modules)
add_subdirectory(include)
add_subdirectory(src)

# We run all tests with the different backends turned on one at a time.

set(NOTIFY_BACKENDS "")

if(NOTIFY_HAVE_EPOLL)
   list(APPEND NOTIFY_BACKENDS EPOLL)
endif()

if(NOTIFY_HAVE_SELECT)
   list(APPEND NOTIFY_BACKENDS SELECT)
endif()

if(NOTIFY_HAVE_POLL)
   list(APPEND NOTIFY_BACKENDS POLL)
endif()

if(NOTIFY_HAVE_EVENT_PORTS)
   list(APPEND NOTIFY_BACKENDS EVPORT)
endif()

if(NOTIFY_HAVE_KQUEUE)
   list(APPEND NOTIFY_BACKENDS KQUEUE)
endif()

message(STATUS "")
message(STATUS "(libnotify ${NOTIFY_VERSION})")
message(STATUS "Available notify backends: ${NOTIFY_BACKENDS}")
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
