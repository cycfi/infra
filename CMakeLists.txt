###############################################################################
#  Copyright (c) 2016-2018 Joel de Guzman
#
#  Distributed under the MIT License (https://opensource.org/licenses/MIT)
###############################################################################
cmake_minimum_required(VERSION 3.7.2)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(common INTERFACE)
target_sources(common INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include/common/support.hpp")
target_include_directories(common INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")



