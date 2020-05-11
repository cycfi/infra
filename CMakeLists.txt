###############################################################################
#  Copyright (c) 2016-2019 Joel de Guzman
#
#  Distributed under the MIT License (https://opensource.org/licenses/MIT)
###############################################################################
cmake_minimum_required(VERSION 3.5.1)

if (TARGET infra)
    return()
endif()

project(infra)

option(INFRA_FORCE_STD_FS "force use of std::filesystem in infra" OFF)
option(INFRA_FORCE_GHC_FS "force use of ghc::filesystem in infra" OFF)

# some ranges of GCC and Clang versions require linker flag
option(INFRA_ADD_FS_LINK "add linker command for standard filesystem library" OFF)

set(INFRA_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(INFRA_FILESYSTEM_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/external/filesystem/include)

add_library(infra INTERFACE)
target_include_directories(infra INTERFACE ${INFRA_HEADER} ${INFRA_FILESYSTEM_HEADER})
target_compile_features(infra INTERFACE cxx_std_14)

if(INFRA_FORCE_STD_FS)
    target_compile_definitions(infra INTERFACE INFRA_FORCE_STD_FS)
endif()

if(INFRA_FORCE_GHC_FS)
    target_compile_definitions(infra INTERFACE INFRA_FORCE_GHC_FS)
endif()

if(INFRA_ADD_FS_LINK)
    target_link_libraries(infra INTERFACE
        $<$<CXX_COMPILER_ID:GNU>:stdc++fs>
        $<$<CXX_COMPILER_ID:Clang>:c++fs>
    )
endif()
