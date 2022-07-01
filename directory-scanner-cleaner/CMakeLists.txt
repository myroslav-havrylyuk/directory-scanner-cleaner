cmake_minimum_required(VERSION 3.16)

project(directory-scanner-cleaner VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.3 COMPONENTS Quick REQUIRED QuickControls2 )

qt_add_executable(appdirectory-scanner-cleaner
	main.cpp
	tools/filesystemmanager.cpp
	tools/configfilehandler.cpp
	tools/filesdeletionhistorymanager.h
	tools/filesdeletionhistorymanager.cpp
	controllers/filesystemcontroller.cpp
	controllers/settingscontroller.cpp
	controllers/mainwindowcontroller.cpp
	controllers/settingswindowcontroller.cpp
	models/filesystemmodel.cpp
	models/filetreeelement.cpp
)

qt_add_qml_module(appdirectory-scanner-cleaner
    URI directory-scanner-cleaner
    VERSION 1.0
	QML_FILES
	views/main.qml
	views/settingswindow.qml
)

set_target_properties(appdirectory-scanner-cleaner PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_compile_definitions(appdirectory-scanner-cleaner
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(appdirectory-scanner-cleaner
	PRIVATE Qt6::Quick
	PRIVATE Qt6::QuickControls2)
