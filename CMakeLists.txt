###############################################################################
#  Copyright (c) 2016-2018 Joel de Guzman
#
#  Distributed under the MIT License (https://opensource.org/licenses/MIT)
###############################################################################
cmake_minimum_required(VERSION 3.5.1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(infra INTERFACE)
target_include_directories(infra INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")

