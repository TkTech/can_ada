cmake_minimum_required(VERSION 3.15...3.30)
project(can_ada LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Python REQUIRED COMPONENTS Interpreter Development.Module)
find_package(nanobind CONFIG REQUIRED)

nanobind_add_module(can_ada src/binding.cpp src/ada.cpp)
target_compile_definitions(can_ada PRIVATE VERSION_INFO=${SKBUILD_PROJECT_VERSION})

install(TARGETS can_ada LIBRARY DESTINATION .)
