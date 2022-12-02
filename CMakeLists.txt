cmake_minimum_required(VERSION 3.16.3)
project(libusb_connect CXX)

set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

include_directories(include)

link_directories(include)

add_library(usb_connect SHARED
	src/usb_connect.cpp
)

set_target_properties(usb_connect PROPERTIES
	PUBLIC_HEADER include/usb_connect.hpp
)

install(TARGETS usb_connect
	EXPORT usb_connect-export
	LIBRARY DESTINATION lib
	INCLUDES DESTINATION include
	PUBLIC_HEADER DESTINATION include/usb_connect
)

install(EXPORT usb_connect-export
	FILE usb_connect-config.cmake
	DESTINATION share/cmake/usb_connect/
	EXPORT_LINK_INTERFACE_LIBRARIES
)

export(EXPORT usb_connect-export
	FILE usb_connect-export.cmake
)
