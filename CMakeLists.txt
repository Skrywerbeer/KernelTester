cmake_minimum_required(VERSION 3.14)

project(KernelTester
	LANGUAGES CXX
	VERSION 0.1
	)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check https://doc.qt.io/qt/deployment-android.html for more information.
# They need to be set before the find_package(...) calls below.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

set(QT_VERSION_MAJOR 6)
set(QT_DEFAULT_MAJOR_VERSION 6)

find_package(QT NAMES Qt6 COMPONENTS
	Core
	Quick
	REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS
	Core
	Quick
	REQUIRED)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

set(PROJECT_SOURCES
        main.cpp
		matview.cpp matview.h
		cameraview.cpp cameraview.h
		kernelmodel.cpp kernelmodel.h
        qml.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(KernelTester
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(KernelTester SHARED
            ${PROJECT_SOURCES}
			)
    else()
        add_executable(KernelTester
          ${PROJECT_SOURCES}
        )
    endif()
endif()

set_target_properties(KernelTester PROPERTIES
	QT_QML_MODULE_VERSION 1.0
	QT_QML_MODULE_URI cam)
qt6_qml_type_registration(KernelTester)
target_compile_definitions(KernelTester
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(KernelTester PRIVATE
	Qt${QT_VERSION_MAJOR}::Core
	Qt${QT_VERSION_MAJOR}::Quick
	${OpenCV_LIBS}
	)
