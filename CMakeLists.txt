cmake_minimum_required(VERSION 3.5)

project(PhotoDistorter LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

add_executable(PhotoDistorter
    headers/mainwidget.h
    headers/mathparser.h
    headers/distortion.h
    headers/listdistortionitem.h
    headers/qtutils.h
    headers/distortionwidget.h
    headers/distortionxwidget.h
    headers/distortionywidget.h
    headers/distortionxywidget.h
    headers/workerdistortion.h
    headers/verticalscrollarea.h
    headers/templatedistortionwidget.h
    headers/templatedistortionwidgetitem.h
    headers/clickablelabel.h
    headers/templatedistortionloader.h
    headers/savedistortionwidget.h
    headers/gridsizewidget.h
    headers/distortionsettings.h
    headers/workspacewidget.h

    src/main.cpp
    src/mainwidget.cpp
    src/mathparser.cpp
    src/distortion.cpp
    src/listdistortionitem.cpp
    src/qtutils.cpp
    src/distortionwidget.cpp
    src/distortionxwidget.cpp
    src/distortionywidget.cpp
    src/distortionxywidget.cpp
    src/workerdistortion.cpp
    src/verticalscrollarea.cpp
    src/templatedistortionwidget.cpp
    src/templatedistortionwidgetitem.cpp
    src/clickablelabel.cpp
    src/templatedistortionloader.cpp
    src/savedistortionwidget.cpp
    src/gridsizewidget.cpp
    src/distortionsettings.cpp
    src/workspacewidget.cpp
)

file(COPY example.jpg
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY icon.png
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY templates
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(PhotoDistorter PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
