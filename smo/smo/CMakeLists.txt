cmake_minimum_required(VERSION 3.5)

project(smo LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

if(ANDROID)
  add_library(smo SHARED
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
  )
else()
  add_executable(smo
    Buffer.cpp
    Buffer.h
    Device.cpp
    Device.h
    Source.cpp
    Source.h
    Request.cpp
    Request.h
    BMS1.cpp
    BMS1.h
    BMS2.cpp
    BMS2.h
    BMS3.cpp
    BMS3.h
    properties.h
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    dialog.h
    dialog.cpp
    dialog.ui
    nextwindow.cpp
    nextwindow.h
    nextwindow.ui
  )
endif()

target_link_libraries(smo PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
