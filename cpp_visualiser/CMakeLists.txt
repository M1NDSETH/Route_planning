cmake_minimum_required(VERSION 3.5)

project(cpp_visualiser VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

# ---------- Путь к алгоритму ----------
set(ALGORITHM_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../cpp)

set(PROJECT_SOURCES
        main.cpp
        canvas.cpp
        canvas.h
        ${ALGORITHM_DIR}/Route.cpp
	${ALGORITHM_DIR}/scenario.cpp
)
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(cpp_visualiser
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(cpp_visualiser SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(cpp_visualiser
            ${PROJECT_SOURCES}
        )
    endif()

endif()

target_link_libraries(cpp_visualiser PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# ---------- Include-пути ----------
target_include_directories(cpp_visualiser PRIVATE
    ${ALGORITHM_DIR}
)

set_target_properties(cpp_visualiser PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(cpp_visualiser)
endif()
