include(CMakeDependentOption)
include(OptionEx)

set(CMAKE_CXX_STANDARD 17 CACHE STRING "Default C++ standard version to be used")
set(CMAKE_CXX_STANDARD 99 CACHE STRING "Default C standard version to be used")

set      (${GLOBAL_NS}_USE_EMPTY_SOURCE_GROUPS OFF CACHE BOOL "If to use virtual directories")
option_ex(${GLOBAL_NS}_ENABLE_GIT_SUBMODULE_CHECK "Check and clone submodules when not available"     ON)
option_ex(${GLOBAL_NS}_ENABLE_LTO                 "Enable link time optimization for Elements targets" OFF)
option_ex(${GLOBAL_NS}_USE_ASAN                   "If to use AddressSanitizer"           OFF)
option_ex(${GLOBAL_NS}_USE_LSAN                   "If to use LeakSanitizer"              OFF)
option_ex(${GLOBAL_NS}_USE_MSAN                   "If to use MemorySanitizer"            OFF)
option_ex(${GLOBAL_NS}_USE_TSAN                   "If to use ThreadSanitizer"            OFF)
option_ex(${GLOBAL_NS}_USE_UBSAN                  "If to use UndefinedBehaviorSanitizer" OFF)

# If we build with Clang, optionally use libc++. Enabled by default on Apple OS.
cmake_dependent_option(${GLOBAL_NS}_USE_LIBCPP "Use libc++ with Clang" "${APPLE}"
   "CMAKE_CXX_COMPILER_ID MATCHES Clang" OFF)
if(${GLOBAL_NS}_USE_LIBCPP)
   add_compile_options(-stdlib=libc++)
   add_link_options(-stdlib=libc++) # New command in CMake 3.13
   add_link_options(-lc++abi)
endif()

if (APPLE)
   if (NOT CMAKE_OSX_ARCHITECTURES)
      set(CMAKE_OSX_ARCHITECTURES ${CMAKE_SYSTEM_PROCESSOR})
   endif()
   if (NOT CMAKE_OSX_ARCHITECTURES MATCHES "arm64|x86_64")
      message(FATAL_ERROR "Unsupported macOS compiler")
   endif()
endif()

# The variable CMAKE_SYSTEM_PROCESSOR is incorrect on Visual studio...
# see https://gitlab.kitware.com/cmake/cmake/issues/15170
if(NOT ${GLOBAL_NS}_SYSTEM_PROCESSOR)
    if(MSVC)
        set(${GLOBAL_NS}_SYSTEM_PROCESSOR "${MSVC_CXX_ARCHITECTURE_ID}" CACHE STRING "" FORCE)
    elseif(APPLE AND CMAKE_OSX_ARCHITECTURES)
        set(${GLOBAL_NS}_SYSTEM_PROCESSOR "${CMAKE_OSX_ARCHITECTURES}" CACHE STRING "" FORCE)
    else()
        set(${GLOBAL_NS}_SYSTEM_PROCESSOR "${CMAKE_SYSTEM_PROCESSOR}" CACHE STRING "" FORCE)
    endif()
endif()
