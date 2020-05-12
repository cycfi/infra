###############################################################################
#  Copyright (c) 2016-2020 Joel de Guzman
#
#  Distributed under the MIT License (https://opensource.org/licenses/MIT)
###############################################################################
cmake_minimum_required(VERSION 3.5.1)

project(infra)

if (NOT TARGET infra)
    set(INFRA_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/include)
    set(INFRA_FILESYSTEM_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/external/filesystem/include)

    add_library(infra INTERFACE)
    target_include_directories(infra INTERFACE ${INFRA_HEADER} ${INFRA_FILESYSTEM_HEADER})
    target_compile_features(infra INTERFACE cxx_std_14)
endif()
