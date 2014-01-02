cmake_minimum_required(VERSION 2.8)
project(elastic-beam)

#General variables setup
set(THIRD_PARTY_DIR "${PROJECT_SOURCE_DIR}/src/third_party")


include_directories("src/")

add_subdirectory("src/power_controller")
