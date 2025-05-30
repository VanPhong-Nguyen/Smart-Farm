cmake_minimum_required(VERSION 3.5)

project(QT5_SmartFarm LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)

if(ANDROID)
  add_library(QT5_SmartFarm SHARED
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    light_sensor.cpp
    light_sensor.h
    temperature_humidity_sensor.cpp
    temperature_humidity_sensor.h
    gpio.cpp
    gpio.h
  )
else()
  add_executable(QT5_SmartFarm
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    light_sensor.cpp
    light_sensor.h
    temperature_humidity_sensor.cpp
    temperature_humidity_sensor.h
    gpio.cpp
    gpio.h
  )
endif()

target_link_libraries(QT5_SmartFarm PRIVATE Qt5::Widgets gpiod)

set(INSTALL_DESTDIR "/opt")

install(TARGETS QT5_SmartFarm
    RUNTIME DESTINATION "${INSTALL_DESTDIR}"
    BUNDLE DESTINATION "${INSTALL_DESTDIR}"
    LIBRARY DESTINATION "${INSTALL_DESTDIR}"
)
