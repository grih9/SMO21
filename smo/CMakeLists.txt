cmake_minimum_required(VERSION 3.16)
project(smo)

set(CMAKE_CXX_STANDARD 14)

add_executable(smo main.cpp Buffer.cpp Buffer.h Device.cpp Device.h Source.cpp Source.h Request.cpp Request.h BMS1.cpp BMS1.h BMS2.cpp BMS2.h BMS3.cpp BMS3.h properties.h)