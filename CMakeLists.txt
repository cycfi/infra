###############################################################################
#  Copyright (c) 2016-2019 Joel de Guzman
#
#  Distributed under the MIT License (https://opensource.org/licenses/MIT)
###############################################################################
cmake_minimum_required(VERSION 3.5.1)

project(infra)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT TARGET infra)
    set(INFRA_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/include)
    set(INFRA_FILESYSTEM_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/external/filesystem/include)

    add_library(infra INTERFACE)
    target_include_directories(infra INTERFACE ${INFRA_HEADER})
endif()