cmake_minimum_required(VERSION 2.8)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

project("Ray Tracer" C CXX)

include_directories(
parsers
math)


file (GLOB rapidjson_headers
parsers/rapidjson/*.h
)

file (GLOB source
main/*.cpp
core/*.cpp
shapes/*.cpp
materials/*.cpp
samplers/*.cpp
)


#raytracer executable
add_executable(raytracer ${source} math/geometry.h ${rapidjson_headers})