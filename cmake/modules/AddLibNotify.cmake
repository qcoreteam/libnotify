
macro(notify_add_executable name)
   cmake_parse_arguments(NOTIFY_ARG "NO_INSTALL_RPATH" ""
                         "DEPENDS" ${ARGN})
   set(ALL_FILES ${NOTIFY_ARG_UNPARSED_ARGUMENTS})
   if(EXCLUDE_FROM_ALL)
      add_executable(${name} EXCLUDE_FROM_ALL ${ALL_FILES})
   else()
      add_executable(${name} ${ALL_FILES})
   endif()
   if(NOT NOTIFY_ARG_NO_INSTALL_RPATH)
      notify_setup_rpath(${name})
   endif()
   notify_add_link_opts(${name})
   # Do not add -Dname_EXPORTS to the command-line when building files in this
   # target. Doing so is actively harmful for the modules build because it
   # creates extra module variants, and not useful because we don't use these
   # macros.
   set_target_properties( ${name} PROPERTIES DEFINE_SYMBOL "" )
   set(EXCLUDE_FROM_ALL OFF)
   set_output_directory(${name}
                        BINARY_DIR ${NOTIFY_RUNTIME_OUTPUT_INTDIR}
                        LIBRARY_DIR ${NOTIFY_LIBRARY_OUTPUT_INTDIR})

   if(NOTIFY_COMMON_DEPENDS)
      add_dependencies(${name} ${NOTIFY_COMMON_DEPENDS})
   endif()
   if(PTHREAD_LIB)
      # libpthreads overrides some standard library symbols, so main
      # executable must be linked with it in order to provide consistent
      # API for all shared libaries loaded by this executable.
      target_link_libraries(${name} ${PTHREAD_LIB})
   endif()
endmacro()

function(notify_add_unittest test_suite test_name)
   if(NOTIFY_OPT_DISABLE_TESTS)
      set(EXCLUDE_FROM_ALL ON)
   endif()
   include_directories(${NOTIFY_UTILS_DIR}/utils/unittest/googletest/include)
   include_directories(${NOTIFY_UTILS_DIR}/utils/unittest/googlemock/include)
   if (NOT NOTIFY_OPT_ENABLE_THREADS)
      list(APPEND NOTIFY_COMPILE_DEFINITIONS GTEST_HAS_PTHREAD=0)
   endif()
   notify_add_executable(${test_name} NO_INSTALL_RPATH ${ARGN})
   set(outdir ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR})
   set_output_directory(${test_name} BINARY_DIR ${outdir} LIBRARY_DIR ${outdir})
   # libpthreads overrides some standard library symbols, so main
   # executable must be linked with it in order to provide consistent
   # API for all shared libaries loaded by this executable.
   target_link_libraries(${test_name} gtest_main gtest ${PTHREAD_LIB} ${CMAKE_PROJECT_NAME})
   add_dependencies(${test_suite} ${test_name})
   get_target_property(test_suite_folder ${test_suite} FOLDER)
   if (NOT ${test_suite_folder} STREQUAL "NOTFOUND")
      set_property(TARGET ${test_name} PROPERTY FOLDER "${test_suite_folder}")
   endif ()
endfunction()

function(notify_setup_rpath name)
   if(CMAKE_INSTALL_RPATH)
      return()
   endif()
   if(NOTIFY_OPT_INSTALL_PREFIX AND NOT (NOTIFY_OPT_INSTALL_PREFIX STREQUAL CMAKE_INSTALL_PREFIX))
      set(extra_libdir ${NOTIFY_OPT_INSTALL_PREFIX})
   elseif(NOTIFY_OPT_INSTALL_PREFIX_DIR)
      set(extra_libdir ${NOTIFY_OPT_INSTALL_PREFIX_DIR})
   endif()
   if(APPLE)
      set(_install_name_dir INSTALL_NAME_DIR "@rpath")
      set(_install_rpath "@load_path/../lib" ${extra_libdir})
   elseif(UNIX)
      set(_install_rpath "\$ORIGIN/../lib${NOTIFY_OPT_LIBDIR_SUFFIX}" ${extra_libdir})
      if(${CMAKE_SYSTEM_NAME} MATCHES "(FreeBSD|DragonFly)")
         set_property(TARGET ${name} APPEND_STRING PROPERTY
                      LINK_FLAGS " -Wl,-z,origin ")
      elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
         set_property(TARGET ${name} APPEND_STRING PROPERTY
                      LINK_FLAGS " -Wl,-rpath-link,${NOTIFY_LIBRARY_OUTPUT_INTDIR} ")
      endif()
   else()
      return()
   endif()
   set_target_properties(${name} PROPERTIES
                         BUILD_WITH_INSTALL_RPATH ON
                         INSTALL_RPATH "${_install_rpath}"
                         ${_install_name_dir})
endfunction()

function(notify_add_link_opts target_name)
   # Don't use linker optimizations in debug builds since it slows down the
   # linker in a context where the optimizations are not important.
   if(NOT NOTIFY_BUILD_TYPE STREQUAL "DEBUG")
      # Pass -O3 to the linker. This enabled different optimizations on different
      # linkers.
      if(NOT (${CMALE_SYSTEM_NAME} MATCHES "Darwin|SunOS|AIX"))
         set_property(TARGET ${target_name} APPEND_STRING PROPERTY
                      LINK_FLAGS " -Wl,-O3")
      endif()
      if(NOTIFY_LINKER_IS_GOLD)
         # With gold gc-sections is always safe.
         set_property(TARGET ${target_name} APPEND_STRING PROPERTY
                      LINK_FLAGS " -Wl,--gc-sections")
         # Note that there is a bug with -Wl,--icf=safe so it is not safe
         # to enable. See https://sourceware.org/bugzilla/show_bug.cgi?id=17704.
      endif()
      
      if(NOT NOTIFY_OPT_NO_DEAD_STRIP)
         if(${CMALE_SYSTEM_NAME} MATCHES "Darwin")
            # ld64's implementation of -dead_strip breaks tools that use plugins.
            set_property(TARGET ${target_name} APPEND_STRING PROPERTY
                         LINK_FLAGS " -Wl,-dead_strip")
         elseif(${CMAKE_SYSTEM_NAME} MATCHES "SunOS")
            set_property(TARGET ${target_name} APPEND_STRING PROPERTY
                         LINK_FLAGS " -Wl,-z -Wl,discard-unused=sections")
         elseif(NOT WIN32 AND NOT NOTIFY_LINKER_IS_GOLD)
            # Object files are compiled with -ffunction-data-sections.
            # Versions of bfd ld < 2.23.1 have a bug in --gc-sections that breaks
            # tools that use plugins. Always pass --gc-sections once we require
            # a newer linker.
            set_property(TARGET ${target_name} APPEND_STRING PROPERTY
                         LINK_FLAGS " -Wl,--gc-sections")
         endif()
      endif()
   endif()
endfunction()

# Set each output directory according to ${CMAKE_CONFIGURATION_TYPES}.
# Note: Don't set variables CMAKE_*_OUTPUT_DIRECTORY any more,
# or a certain builder, for eaxample, msbuild.exe, would be confused.
function(set_output_directory target)
   cmake_parse_arguments(ARG "" "BINARY_DIR;LIBRARY_DIR" "" ${ARGN})
   
   # module_dir -- corresponding to LIBRARY_OUTPUT_DIRECTORY.
   # It affects output of add_library(MODULE).
   if(WIN32 OR CYGWIN)
      # DLL platform
      set(module_dir ${ARG_BINARY_DIR})
   else()
      set(module_dir ${ARG_LIBRARY_DIR})
   endif()
   if(NOT "${CMAKE_CFG_INTDIR}" STREQUAL ".")
      foreach(build_mode ${CMAKE_CONFIGURATION_TYPES})
         string(TOUPPER "${build_mode}" CONFIG_SUFFIX)
         if(ARG_BINARY_DIR)
            string(REPLACE ${CMAKE_CFG_INTDIR} ${build_mode} bi ${ARG_BINARY_DIR})
            set_target_properties(${target} PROPERTIES "RUNTIME_OUTPUT_DIRECTORY_${CONFIG_SUFFIX}" ${bi})
         endif()
         if(ARG_LIBRARY_DIR)
            string(REPLACE ${CMAKE_CFG_INTDIR} ${build_mode} li ${ARG_LIBRARY_DIR})
            set_target_properties(${target} PROPERTIES "ARCHIVE_OUTPUT_DIRECTORY_${CONFIG_SUFFIX}" ${li})
         endif()
         if(module_dir)
            string(REPLACE ${CMAKE_CFG_INTDIR} ${build_mode} mi ${module_dir})
            set_target_properties(${target} PROPERTIES "LIBRARY_OUTPUT_DIRECTORY_${CONFIG_SUFFIX}" ${mi})
         endif()
      endforeach()
   else()
      if(ARG_BINARY_DIR)
         set_target_properties(${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${ARG_BINARY_DIR})
      endif()
      if(ARG_LIBRARY_DIR)
         set_target_properties(${target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${ARG_LIBRARY_DIR})
      endif()
      if(module_dir)
         set_target_properties(${target} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${module_dir})
      endif()
   endif()
endfunction()