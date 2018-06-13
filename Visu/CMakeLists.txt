cmake_minimum_required(VERSION 2.9)
project(visu)


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)


find_package(Qt5Widgets REQUIRED)
find_package(Qt5PrintSupport REQUIRED)

include_directories(${Qt5Widgets_INCLUDES} ./include/ ../include  ${LIB_SHM_PATH}/include ${LIB_QCUSTOMPLOT_PATH}/include  ${LIB_DECODEFRAME_PATH}/include /usr/local/include)

add_definitions(${Qt5Widgets_DEFINITIONS})

qt5_wrap_ui(UIS_HDRS
    src/mainwindow.ui
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
	visu 
	${source_files}
	${UIS_HDRS}
)

target_link_libraries(visu ${Qt5Widgets_LIBRARIES} Qt5::PrintSupport ringshm qcustomplot frame)




