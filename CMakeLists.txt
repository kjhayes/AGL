#Copyright (C) - Kevin Hayes - 2022 - <KevinHayes2026@u.northwestern.edu> - All Rights Reserved

set(AGL_MINIMUM_CMAKE_VERSION 3.14)
cmake_minimum_required(VERSION ${AGL_MINIMUM_CMAKE_VERSION})

project(abstractgl)

set(AGL_LIB agl)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/source)

set_target_properties(${AGL_LIB} PROPERTIES CXX_STANDARD 17)
set_target_properties(${AGL_LIB} PROPERTIES CXX_STANDARD_REQUIRED true)