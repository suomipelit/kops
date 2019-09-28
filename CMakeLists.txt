cmake_minimum_required(VERSION 3.0)
project(kops VERSION 1.0.0)

set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR}/cmake-modules)

if(NOT EXISTS "${CMAKE_BINARY_DIR}/cmake-modules/FindSDL2.cmake")
	file(DOWNLOAD "https://github.com/tcbrindle/sdl2-cmake-scripts/raw/master/FindSDL2.cmake" "${CMAKE_BINARY_DIR}/cmake-modules/FindSDL2.cmake")
endif()
if(NOT EXISTS "${CMAKE_BINARY_DIR}/cmake-modules/FindSDL2_mixer.cmake")
	file(DOWNLOAD "https://github.com/tcbrindle/sdl2-cmake-scripts/raw/master/FindSDL2_mixer.cmake" "${CMAKE_BINARY_DIR}/cmake-modules/FindSDL2_mixer.cmake")
endif()
if(NOT EXISTS "${CMAKE_BINARY_DIR}/cmake-modules/FindZLIB.cmake")
	file(DOWNLOAD "https://raw.githubusercontent.com/Kitware/CMake/master/Modules/FindZLIB.cmake" "${CMAKE_BINARY_DIR}/cmake-modules/FindZLIB.cmake")
endif()
if(NOT EXISTS "${CMAKE_BINARY_DIR}/cmake-modules/SelectLibraryConfigurations.cmake")
	file(DOWNLOAD "https://raw.githubusercontent.com/Kitware/CMake/master/Modules/SelectLibraryConfigurations.cmake" "${CMAKE_BINARY_DIR}/cmake-modules/SelectLibraryConfigurations.cmake")
endif()
if(NOT EXISTS "${CMAKE_BINARY_DIR}/cmake-modules/FindPackageHandleStandardArgs.cmake")
	file(DOWNLOAD "https://raw.githubusercontent.com/Kitware/CMake/master/Modules/FindPackageHandleStandardArgs.cmake" "${CMAKE_BINARY_DIR}/cmake-modules/FindPackageHandleStandardArgs.cmake")
endif()
if(NOT EXISTS "${CMAKE_BINARY_DIR}/cmake-modules/FindPackageMessage.cmake")
	file(DOWNLOAD "https://raw.githubusercontent.com/Kitware/CMake/master/Modules/FindPackageMessage.cmake" "${CMAKE_BINARY_DIR}/cmake-modules/FindPackageMessage.cmake")
endif()

find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(ZLIB REQUIRED)

if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug)
endif()

if (WIN32)
	add_compile_options(/MP /MT /WX /wd4996)
    else()
	# Warnings to be fixed
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
endif()

file(GLOB SOURCES
	src/*.h src/*.c
	src/util/*.h src/util/*.c
	src/kops-bot/reactive-agent/*.h src/kops-bot/reactive-agent/*.c)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/SRC
	${SDL2_INCLUDE_DIR}
	${SDL2_MIXER_INCLUDE_DIRS}
	${ZLIB_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME}
	${SDL2_LIBRARY}
	${SDL2_MIXER_LIBRARIES}
	${ZLIB_LIBRARIES})

if (WIN32)
	set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "KOPS" SUFFIX ".exe")
	target_link_libraries(${PROJECT_NAME} winmm.lib)
elseif (UNIX)
	target_link_libraries(${PROJECT_NAME} m)
endif()

if (APPLE)
	target_compile_definitions(${PROJECT_NAME} PRIVATE _POSIX_SOURCE=1 PATH_MAX=260)
endif()
