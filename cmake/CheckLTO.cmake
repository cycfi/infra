include(CheckIPOSupported)
check_ipo_supported(RESULT IPO_SUPPORTED OUTPUT IPO_ERROR)

option(${GLOBAL_NS}_ENABLE_LTO "Enable link time optimization for Elements targets." OFF)

set(_lto_test true)

if (NOT IPO_SUPPORTED)
   message(WARNING "Link-time optimization not supported: ${IPO_ERROR}")
   set(_lto_test false)
elseif(CMAKE_VERSION VERSION_LESS 3.9)
   message(WARNING "IPO/LTO checks are only available on CMake 3.9 and later.")
   set(_lto_test false)
elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL armv7l)
   message(WARNING "IPO/LTO not available for armv7l.")
   set(_lto_test false)
endif()

if (NOT _lto_test)
   set(${GLOBAL_NS}_ENABLE_LTO OFF CACHE BOOL "" FORCE)

   function(enable_lto_if_available_for TARGET) # Do nothing.
   endfunction()

   unset(_lto_test)
   return()
else()
   function(enable_lto_if_available_for TARGET)
      if (CMAKE_BUILD_TYPE STREQUAL "Release")
         message(STATUS "Link-time optimization enabled for ${TARGET}.")
         set_target_properties(artist PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
      else()
         message(STATUS "Link-time optimization disabled for ${TARGET}: not a Release build.")
         set(${GLOBAL_NS}_ENABLE_LTO OFF CACHE BOOL "" FORCE)
      endif()
   endfunction()
endif()

unset(_lto_test)
