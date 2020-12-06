cmake_minimum_required(VERSION 3.10)

project(__PROJNAME__
	VERSION 0.01
	LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
   add_compile_options (-fdiagnostics-color=always)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
   add_compile_options (-fcolor-diagnostics)
endif ()

find_package(Doxygen)
if (DOXYGEN_FOUND)
	add_subdirectory(docs)
else()
	message(STATUS "Doxygen not found, skipping docs")
endif()

add_subdirectory(extern)
add_subdirectory(src)
add_subdirectory(__PROJNAME__)