# Don't show build information when building a different project
if(NOT PROJECT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
   function(show_build_info_if_needed)
   endfunction()
   return()
endif()

function(show_build_info_if_needed)
   set(${GLOBAL_NS}_HOST_SYSTEM_NAME ${CMAKE_HOST_SYSTEM_NAME})
   if (${GLOBAL_NS}_HOST_SYSTEM_NAME STREQUAL "Darwin")
      set(${GLOBAL_NS}_HOST_SYSTEM_NAME "macOS")
   endif()
   message(STATUS "
Project name:                  ${PROJECT_NAME}
Host system name:              ${${GLOBAL_NS}_HOST_SYSTEM_NAME} ${CMAKE_HOST_SYSTEM_VERSION}
C++ standard version:          ${CMAKE_CXX_STANDARD}
Build type:                    ${CMAKE_BUILD_TYPE}
Build processor:               ${${GLOBAL_NS}_SYSTEM_PROCESSOR}
Build using LTO:               ${${GLOBAL_NS}_ENABLE_LTO}

Use clang libc++:              ${${GLOBAL_NS}_USE_LIBCPP}
Use ASAN:                      ${${GLOBAL_NS}_USE_ASAN}
Use LSAN:                      ${${GLOBAL_NS}_USE_LSAN}
Use MSAN:                      ${${GLOBAL_NS}_USE_MSAN}
Use TSAN:                      ${${GLOBAL_NS}_USE_TSAN}
Use UBSAN:                     ${${GLOBAL_NS}_USE_UBSAN}

C compiler:                    ${CMAKE_C_COMPILER}
C compiler id:                 ${CMAKE_C_COMPILER_ID}
C compiler version:            ${CMAKE_C_COMPILER_VERSION}

CPP compiler:                  ${CMAKE_CXX_COMPILER}
CPP compiler id:               ${CMAKE_CXX_COMPILER_ID}
CPP compiler version:          ${CMAKE_CXX_COMPILER_VERSION}

Install prefix:                ${CMAKE_INSTALL_PREFIX}")

if (CMAKE_OSX_DEPLOYMENT_TARGET)
   message(STATUS "macOS deployment target:       ${CMAKE_OSX_DEPLOYMENT_TARGET}")
endif()
endfunction()
