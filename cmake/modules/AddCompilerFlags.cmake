include(CheckCXXCompilerFlag)

macro(notify_add_compiler_flags)
   foreach(flag ${ARGN})
      string(REGEX REPLACE "[-.+/:= ]" "_" _flag_esc ${flag})
      check_cxx_compiler_flag("${flag}" check_cxx_compiler_flag_${_flag_esc})
      if(check_cxx_compiler_flag_${_flag_esc})
         set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}")
      endif()
   endforeach()
endmacro()

