cmake_minimum_required(VERSION 2.9)
project(visuqwt)


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)


find_package(Qt5Widgets REQUIRED)
find_package(Qt5PrintSupport REQUIRED)
find_package(Qt5Network REQUIRED)

include_directories(${Qt5Widgets_INCLUDES} ./include/ ../include  ${LIB_SHM_PATH}/include /usr/local/qwt/include  ${LIB_DECODEFRAME_PATH}/include /usr/local/include)

add_definitions(${Qt5Widgets_DEFINITIONS} )

qt5_wrap_ui(UIS_HDRS
    src/mainwindow.ui
)


set(QWT_LIBRARY_NAMES qwt-qt5 qwt6-qt5 qwt qwt6)
find_library(QWT_LIBRARIES
  NAMES ${QWT_LIBRARY_NAMES}
  PATHS
    /usr/lib
    /usr/local/qwt/lib
    /usr/local/lib/qt5
    "$ENV{LIB_DIR}/lib"
    "$ENV{LIB}"
)

set(CMAKE_CXX_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

file (
	GLOB_RECURSE

	source_files
	src/*.c
	src/*.cpp
        
	include/*.h     
	../include/*.h
)


add_executable(
	visuqwt 
	${source_files}
	${UIS_HDRS}
)

target_link_libraries(visuqwt ${Qt5Widgets_LIBRARIES} Qt5::PrintSupport Qt5::Network ringshm ${QWT_LIBRARIES} frame)




