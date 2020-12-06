project(lib__PROJNAME__ CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D___PROJNAMEUPPER__DEBUG")

if(WIN32)
    macro(get_WIN32_WINNT version)
        if(CMAKE_SYSTEM_VERSION)
            set(ver ${CMAKE_SYSTEM_VERSION})
            string(REGEX MATCH "^([0-9]+).([0-9])" ver ${ver})
            string(REGEX MATCH "^([0-9]+)" verMajor ${ver})
            # Check for Windows 10, b/c we'll need to convert to hex 'A'.
            if("${verMajor}" MATCHES "10")
                set(verMajor "A")
                string(REGEX REPLACE "^([0-9]+)" ${verMajor} ver ${ver})
            endif()
            # Remove all remaining '.' characters.
            string(REPLACE "." "" ver ${ver})
            # Prepend each digit with a zero.
            string(REGEX REPLACE "([0-9A-Z])" "0\\1" ver ${ver})
            set(${version} "0x${ver}")
        endif()
    endmacro()

    get_WIN32_WINNT(ver)
    add_definitions(-D_WIN32_WINNT=${ver})
endif()

file(GLOB_RECURSE sourceFiles ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(lib__PROJNAME__ ${sourceFiles})

target_include_directories(lib__PROJNAME__ 
	PUBLIC ${CMAKE_SOURCE_DIR}/include
	PUBLIC ${CMAKE_SOURCE_DIR}/extern/asio/asio/include)

target_link_libraries(lib__PROJNAME__
    PUBLIC spdlog::spdlog)