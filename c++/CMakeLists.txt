cmake_minimum_required(VERSION 3.15)

project(
        CPP-Raytracing
	VERSION 0.1
        DESCRIPTION "Raytracing in one weekend, my c++ implementation"
	LANGUAGES CXX
)

if(CMAKE_BUILD_TYPE)
        message(STATUS "Building in ${CMAKE_BUILD_TYPE} mode")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
        set(CMAKE_CXX_FLAGS_RELEASE  "-Ofast -funsafe-math-optimizations -march=native")
endif()
message(STATUS "CXXFLAGS = ${CMAKE_CXX_FLAGS_RELEASE}")

find_package (Eigen3 REQUIRED NO_MODULE)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(CheckIPOSupported)

add_subdirectory(src)

# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

	set(CMAKE_CXX_EXTENSIONS OFF)
	set_property(GLOBAL PROPERTY USE_FOLDERS ON)

	find_package(Doxygen)
	if(Doxygen_FOUND)
		add_subdirectory(docs)
	else()
		message(STATUS "Doxygen not found, not building docs")
	endif()

	set(WFLAGS -Werror -Wall -Wextra -pedantic -Wshadow -Wnon-virtual-dtor -Wold-style-cast -Wcast-align -Wunused -Woverloaded-virtual -Wpedantic -Wconversion -Wsign-conversion -Wmisleading-indentation -Wnull-dereference -Wdouble-promotion -Wformat=2 -Weffc++)
	if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		set(WFLAGS ${WFLAGS} -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wuseless-cast)
	endif()
endif() # Main project only part ends here

if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MODERN_CMAKE_BUILD_TESTING)
    AND BUILD_TESTING)
	add_subdirectory(tests)
endif()
