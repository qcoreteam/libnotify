# notify_add_library(name sources...
#   SHARED;STATIC
#     STATIC by default w/o BUILD_SHARED_LIBS.
#     SHARED by default w/  BUILD_SHARED_LIBS.
#   OBJECT
#     Also create an OBJECT library target. Default if STATIC && SHARED.
#   MODULE
#     Target ${name} might not be created on unsupported platforms.
#     Check with "if(TARGET ${name})".
#   OUTPUT_NAME name
#     Corresponds to OUTPUT_NAME in target properties.
#   DEPENDS targets...
#     Same semantics as add_dependencies().
#   LINK_COMPONENTS components...
#     Same as the variable LLVM_LINK_COMPONENTS.
#   LINK_LIBS lib_targets...
#     Same semantics as target_link_libraries().
#   ADDITIONAL_HEADERS
#     May specify header files for IDE generators.
#   SONAME
#     Should set SONAME link flags and create symlinks
#   )
function(notify_add_library name)
   cmake_parse_arguments(NOTIFY_ARG
                         "MODULE;SHARED;STATIC;OBJECT;SONAME"
                         "OUTPUT_NAME"
                         "ADDITIONAL_HEADERS;DEPENDS;LINK_COMPONENTS;LINK_LIBS;OBJLIBS"
                         ${ARGN})
   list(APPEND NOTIFY_COMMON_DEPENDS ${NOTIFY_ARG_DEPENDS})
   if(NOTIFY_ARG_ADDITIONAL_HEADERS)
      # Pass through ADDITIONAL_HEADERS.
      set(NOTIFY_ARG_ADDITIONAL_HEADERS ADDITIONAL_HEADERS ${NOTIFY_ARG_ADDITIONAL_HEADERS})
   endif()
   if(NOTIFY_ARG_OBJLIBS)
      set(ALL_FILES ${NOTIFY_ARG_OBJLIBS})
   else()
      list(APPEND ALL_FILES ${NOTIFY_ARG_UNPARSED_ARGUMENTS} ${NOTIFY_ARG_ADDITIONAL_HEADERS})
   endif()
   
   if(NOTIFY_ARG_MODULE)
      if(NOTIFY_ARG_SHARED OR NOTIFY_ARG_STATIC)
         message(WARNING "MODULE with SHARED|STATIC doesn't make sense.")
      endif()
   else()
      if(BUILD_SHARED_LIBS AND NOT NOTIFY_ARG_STATIC)
         set(NOTIFY_ARG_SHARED TRUE)
      endif()
      if(NOT NOTIFY_ARG_SHARED)
         set(NOTIFY_ARG_STATIC TRUE)
      endif()
   endif()
   # Generate objlib
   if((NOTIFY_ARG_SHARED AND NOTIFY_ARG_STATIC) OR NOTIFY_OBJECT)
      # Generate an obj library for both targets.
      set(obj_name "obj.${name}")
      add_library(${obj_name} OBJECT EXCLUDE_FROM_ALL
                  ${ALL_FILES})
      set(ALL_FILES "$<TARGET_OBJECTS:${obj_name}>")
      # Do add_dependencies(obj) later due to CMake issue 14747.
      set_target_properties(${obj_name} PROPERTIES FOLDER "Object Libraries")
   endif()

   if(NOTIFY_ARG_SHARED AND NOTIFY_ARG_STATIC)
      # static
      set(name_static "${name}_static")
      if(NOTIFY_ARG_OUTPUT_NAME)
         set(output_name OUTPUT_NAME "${NOTIFY_ARG_OUTPUT_NAME}")
      endif()
      # DEPENDS has been appended to NOTIFY_COMMON_LIBS.
      notify_add_library(${name_static} STATIC
                         ${output_name}
                         OBJLIBS ${ALL_FILES} # objlib
                         LINK_LIBS ${NOTIFY_ARG_LINK_LIBS}
                         LINK_COMPONENTS ${NOTIFY_ARG_LINK_COMPONENTS})
      # FIXME: Add name_static to anywhere in TARGET ${name}'s PROPERTY.
      set(NOTIFY_ARG_STATIC)
   endif()
   
   if(NOTIFY_ARG_MODULE)
      add_library(${name} MODULE ${ALL_FILES})
      notify_setup_rpath(${name})
   elseif(NOTIFY_ARG_SHARED)
      add_library(${name} SHARED ${ALL_FILES})
      notify_setup_rpath(${name})
   else()
      add_library(${name} STATIC ${ALL_FILES})
   endif()
   set_output_directory(${name}
                        BINARY_DIR ${NOTIFY_RUNTIME_OUTPUT_INTDIR}
                        LIBRARY_DIR ${NOTIFY_LIBRARY_OUTPUT_INTDIR})
   # $<TARGET_OBJECTS> doesn't require compile flags.
   if(NOTIFY_ARG_OUTPUT_NAME)
      set_target_properties(${name}
                            PROPERTIES
                            OUTPUT_NAME ${NOTIFY_ARG_OUTPUT_NAME})
   endif()
   
   if(NOTIFY_ARG_MODULE)
      set_target_properties(${name}
                            PROPERTIES
                            PREFIX ""
                            SUFFIX ${NOTIFY_PLUGIN_EXT})
   endif()
   
   if(NOTIFY_ARG_SHARED)
      if(WIN32)
         set_target_properties(${name} PROPERTIES
                               PREFIX "")
         # Set SOVERSION on shared libraries that lack explicit SONAME
         # specifier, on *nix systems that are not Darwin.
         if(UNIX AND NOT APPLE AND NOT NOTIFY_ARG_SONAME)
            set_target_properties(${name}
                                  PROPERTIES
                                  # Since 1.0.0, the ABI version is indicated by the major version
                                  SOVERSION ${NOTIFY_VERSION_MAJOR}
                                  VERSION ${NOTIFY_VERSION_MAJOR}.${NOTIFY_VERSION_MINOR}.${NOTIFY_VERSION_PATCH}${NOTIFY_VERSION_STAGE})
         endif()
      endif()
   endif()
endfunction()

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
   include_directories(${NOTIFY_UTILS_DIR}/unittest/googletest/include)
   include_directories(${NOTIFY_UTILS_DIR}/unittest/googlemock/include)
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


# Set each output directory according to ${CMAKE_CONFIGURATION_TYPES}.
# Note: Don't set variables CMAKE_*_OUTPUT_DIRECTORY any more,
# or a certain builder, for eaxample, msbuild.exe, would be confused.
function(set_output_directory target)
   cmake_parse_arguments(NOTIFY_ARG "" "BINARY_DIR;LIBRARY_DIR" "" ${ARGN})
   
   # module_dir -- corresponding to LIBRARY_OUTPUT_DIRECTORY.
   # It affects output of add_library(MODULE).
   if(WIN32 OR CYGWIN)
      # DLL platform
      set(module_dir ${NOTIFY_ARG_BINARY_DIR})
   else()
      set(module_dir ${NOTIFY_ARG_LIBRARY_DIR})
   endif()
   if(NOT "${CMAKE_CFG_INTDIR}" STREQUAL ".")
      foreach(build_mode ${CMAKE_CONFIGURATION_TYPES})
         string(TOUPPER "${build_mode}" CONFIG_SUFFIX)
         if(NOTIFY_ARG_BINARY_DIR)
            string(REPLACE ${CMAKE_CFG_INTDIR} ${build_mode} bi ${NOTIFY_ARG_BINARY_DIR})
            set_target_properties(${target} PROPERTIES "RUNTIME_OUTPUT_DIRECTORY_${CONFIG_SUFFIX}" ${bi})
         endif()
         if(NOTIFY_ARG_LIBRARY_DIR)
            string(REPLACE ${CMAKE_CFG_INTDIR} ${build_mode} li ${NOTIFY_ARG_LIBRARY_DIR})
            set_target_properties(${target} PROPERTIES "ARCHIVE_OUTPUT_DIRECTORY_${CONFIG_SUFFIX}" ${li})
         endif()
         if(module_dir)
            string(REPLACE ${CMAKE_CFG_INTDIR} ${build_mode} mi ${module_dir})
            set_target_properties(${target} PROPERTIES "LIBRARY_OUTPUT_DIRECTORY_${CONFIG_SUFFIX}" ${mi})
         endif()
      endforeach()
   else()
      if(NOTIFY_ARG_BINARY_DIR)
         set_target_properties(${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${NOTIFY_ARG_BINARY_DIR})
      endif()
      if(NOTIFY_ARG_LIBRARY_DIR)
         set_target_properties(${target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${NOTIFY_ARG_LIBRARY_DIR})
      endif()
      if(module_dir)
         set_target_properties(${target} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${module_dir})
      endif()
   endif()
endfunction()

function(notify_update_compile_flags name)
   get_property(sources TARGET ${name} PROPERTY SOURCES)
   if("${sources}" MATCHES "\\.c(;|$)")
      set(update_src_props ON)
   endif()
   ## NOTIFY_REQUIRES_EXCEPTION is an internal flag individual targets can use to
   ## force Exception handle
   if(NOTIFY_REQUIRES_EXCEPTION OR NOT NOTIFY_OPT_DISABLE_EXCEPTION)
      if(NOT (NOTIFY_REQUIRES_RTTI OR NOT NOTIFY_OPT_DISABLE_RTTI))
         message(AUTHOR_WARNING "Exception handling requires RTTI. Enabling RTTI for ${name}")
         set(NOTIFY_REQUIRES_RTTI ON)
      endif()
   else()
   
   endif()
endfunction()