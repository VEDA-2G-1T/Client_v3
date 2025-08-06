cmake_minimum_required(VERSION 3.16)

project(Client_v3 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Multimedia MultimediaWidgets WebSockets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Multimedia MultimediaWidgets WebSockets)

# ==================== ## OpenCV 설정 시작 ## ====================

# 1. OpenCV 패키지 찾기 (PATHS 경로는 실제 OpenCV 설치 경로로 수정하세요!)
find_package(OpenCV REQUIRED PATHS "C:/opencv/build")

# 2. OpenCV 헤더 파일 경로 포함
if(OpenCV_FOUND)
    include_directories(${OpenCV_INCLUDE_DIRS})
endif()

# ==================== ## OpenCV 설정 끝 ## ====================

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    find_package(Qt6 REQUIRED COMPONENTS Widgets)

    qt_add_executable(Client_v3
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        resources.qrc
        aspectratiowidget.h aspectratiowidget.cpp
        dragdropimagelabel.h dragdropimagelabel.cpp
        custommessagebox.h custommessagebox.cpp
        debug_utils.h
        exitconfirmdialog.h exitconfirmdialog.cpp
        loginpopup.h loginpopup.cpp
        cameraregistrationdialog.h cameraregistrationdialog.cpp
        brightnessdialog.h brightnessdialog.cpp
        notificationitemwidget.h notificationitemwidget.cpp
        notificationlistpanel.h notificationlistpanel.cpp
        loginwidget.h loginwidget.cpp
        loginwidget.cpp loginwidget.h
        customtabbar.h customtabbar.cpp
        eventcard.h eventcard.cpp
        eventcardpopupdialog.h eventcardpopupdialog.cpp
        continuousdetectionpopupdialog.h continuousdetectionpopupdialog.cpp
        fullscreencameradialog.h fullscreencameradialog.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Client_v3 APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(Client_v3 SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Client_v3
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(Client_v3 PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Multimedia
    Qt${QT_VERSION_MAJOR}::MultimediaWidgets
    Qt${QT_VERSION_MAJOR}::WebSockets
     ${OpenCV_LIBS}
    )
target_link_libraries(Client_v3 PRIVATE Qt6::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Client_v3)
endif()
set_target_properties(Client_v3 PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Client_v3
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Client_v3)
endif()
