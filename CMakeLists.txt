cmake_minimum_required(VERSION 3.25)

project(Gctk)
project(GctkGtex)
project(GctkMath)
project(GctkTest)
project(StrLib)

set(CMAKE_CXX_STANDARD 20)

if (WIN32)
	add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
endif ()

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)

set(GLM_ENABLE_CXX_20 ON)

add_subdirectory(submodules/fmt)
add_subdirectory(submodules/glm)

target_compile_options(glm PUBLIC -Wno-unsafe-buffer-usage)
target_compile_definitions(glm PUBLIC GLM_ENABLE_EXPERIMENTAL)

file(GLOB GCTK_SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

add_library(Gctk STATIC ${GCTK_SOURCES})
add_library(Gctk::Gctk ALIAS Gctk)
add_library(GctkGtex STATIC ${CMAKE_CURRENT_LIST_DIR}/src/gtex/gctk_gtex.cpp)
add_library(Gctk::Gtex ALIAS GctkGtex)
add_library(GctkMath STATIC ${CMAKE_CURRENT_LIST_DIR}/src/math/gctk_math.cpp)
add_library(Gctk::Math ALIAS GctkMath)
add_library(StrLib STATIC ${CMAKE_CURRENT_LIST_DIR}/src/string/string.cpp)
add_library(Gctk::StrLib ALIAS StrLib)

target_include_directories(StrLib PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include/strlib)

target_link_libraries(StrLib PUBLIC fmt::fmt)
target_link_libraries(GctkGtex PUBLIC OpenGL::GL GLEW::GLEW)
target_link_libraries(GctkMath PUBLIC glm::glm  Gctk::StrLib)
target_link_libraries(Gctk PUBLIC OpenGL::GL SDL2::SDL2 SDL2::SDL2main GLEW::GLEW GctkMath GctkGtex Gctk::StrLib)

add_executable(GctkTest test/main.cpp)
target_link_libraries(GctkTest PRIVATE Gctk::Gctk)