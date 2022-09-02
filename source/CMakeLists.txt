#Copyright (C) - Kevin Hayes - 2022 - All Rights Reserved

cmake_minimum_required(VERSION 3.14)

file(GLOB AGL_LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(${AGL_LIB} STATIC ${AGL_LIB_SOURCES})

target_include_directories(${AGL_LIB} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)

#Opengl
if(WIN32)
    set(OPENGL_LIB opengl32)
else()
    set(OPENGL_LIB gl)
endif()
target_link_libraries(${AGL_LIB} PRIVATE ${OPENGL_LIB})

#Opengl loader
find_package(glad CONFIG REQUIRED)
target_link_libraries(${AGL_LIB} PRIVATE glad::glad)

#Opengl Math
find_package(glm CONFIG REQUIRED)
target_link_libraries(${AGL_LIB} PRIVATE glm::glm)