cmake_minimum_required(VERSION 3.13 FATAL_ERROR) # target_link_directories

project(tray C)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

# Generate 'compile_commands.json' for clang_complete
set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT WIN32)
	find_package (PkgConfig REQUIRED)
else()
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
endif()

# Sources for libtray
list(APPEND SRCS ${CMAKE_CURRENT_SOURCE_DIR}/tray.h)
if(WIN32)
	list(APPEND SRCS ${CMAKE_CURRENT_SOURCE_DIR}/tray_windows.c)
else()
	if(UNIX)
		if(APPLE)
			list(APPEND SRCS ${CMAKE_CURRENT_SOURCE_DIR}/tray_darwin.m)
		else()
			list(APPEND SRCS ${CMAKE_CURRENT_SOURCE_DIR}/tray_linux.c)
		endif()
	endif()
endif()

add_library(tray SHARED ${SRCS})
set_property(TARGET tray PROPERTY C_STANDARD 99)
target_compile_definitions(tray PRIVATE TRAY_EXPORTS)
set_target_properties(tray PROPERTIES C_VISIBILITY_PRESET hidden)

if(WIN32)
	target_compile_definitions(tray PRIVATE TRAY_WINAPI=1 WIN32_LEAN_AND_MEAN NOMINMAX)
else()
	if(UNIX)
		if(APPLE)
			find_library(COCOA Cocoa REQUIRED)
			target_link_libraries(tray PRIVATE ${COCOA})
			target_compile_definitions(tray PRIVATE TRAY_APPKIT=1)
		else()
			find_package(APPINDICATOR REQUIRED)
			target_compile_options(tray PRIVATE ${APPINDICATOR_CFLAGS})
			target_link_directories(tray PRIVATE ${APPINDICATOR_LIBRARY_DIRS})
			target_compile_definitions(tray PRIVATE TRAY_APPINDICATOR=1)
			target_link_libraries(tray PRIVATE ${APPINDICATOR_LIBRARIES})
		endif()
	endif()
endif()

INSTALL(TARGETS tray DESTINATION lib)

IF(NOT WIN32)
	INSTALL(FILES tray.h DESTINATION include)
ENDIF()


# Tray Example
add_executable(tray_example ${CMAKE_CURRENT_SOURCE_DIR}/example.c)
target_link_libraries(tray_example PRIVATE tray)

if(WIN32)
	configure_file(${CMAKE_CURRENT_LIST_DIR}/icon.ico ${CMAKE_BINARY_DIR}/icon.ico COPYONLY)
	configure_file(${CMAKE_CURRENT_LIST_DIR}/icon2.ico ${CMAKE_BINARY_DIR}/icon2.ico COPYONLY)
else()
	if(UNIX)
		if(APPLE)
			configure_file(${CMAKE_CURRENT_LIST_DIR}/icon.png ${CMAKE_BINARY_DIR}/icon.png COPYONLY)
			configure_file(${CMAKE_CURRENT_LIST_DIR}/icon2.png ${CMAKE_BINARY_DIR}/icon2.png COPYONLY)
		else ()
			configure_file(${CMAKE_CURRENT_LIST_DIR}/icon-24px.png ${CMAKE_BINARY_DIR}/icon-24px.png COPYONLY)
			configure_file(${CMAKE_CURRENT_LIST_DIR}/icon2-24px.png ${CMAKE_BINARY_DIR}/icon2-24px.png COPYONLY)
		endif()
	endif()
endif ()
