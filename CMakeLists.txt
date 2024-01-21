cmake_minimum_required(VERSION 3.25)

project(Gctk)
project(GctkGtex)
project(GctkMath)
project(GctkTest)

set(CMAKE_CXX_STANDARD 20)

if (WIN32)
	add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
endif ()

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)

add_subdirectory(submodules/fmt)
add_subdirectory(submodules/glm)

file(GLOB GCTK_SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

add_library(Gctk STATIC ${GCTK_SOURCES})
add_library(Gctk::Gctk ALIAS Gctk)
add_library(GctkGtex STATIC ${CMAKE_CURRENT_LIST_DIR}/src/gtex/gctk_gtex.cpp)
add_library(Gctk::Gtex ALIAS GctkGtex)
add_library(GctkMath STATIC ${CMAKE_CURRENT_LIST_DIR}/src/math/gctk_math.cpp)
add_library(Gctk::Math ALIAS GctkMath)

target_link_libraries(GctkGtex PUBLIC OpenGL::GL GLEW::GLEW)
target_link_libraries(GctkMath PUBLIC glm::glm fmt::fmt)
target_link_libraries(Gctk PUBLIC OpenGL::GL SDL2::SDL2 SDL2::SDL2main GLEW::GLEW GctkMath GctkGtex fmt::fmt)

add_executable(GctkTest test/main.cpp)
target_link_libraries(GctkTest PRIVATE Gctk::Gctk)