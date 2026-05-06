cmake_minimum_required(VERSION 4.2)
project(cur8-proto VERSION 0.0.1 LANGUAGES C CXX)

include(CTest)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

#required due to tested toolchains not handling the newer boost config cmake approach correctly
if(POLICY CMP0167)
    cmake_policy(SET CMP0167 OLD)
endif()
if(POLICY CMP0144)
    cmake_policy(SET CMP0144 NEW)
endif()

find_package(Boost 1.91.0 REQUIRED COMPONENTS
    json
)

add_library(SQLite3 STATIC ${SQLite3_DIR}/sqlite3.c)
set_target_properties(SQLite3 PROPERTIES LINKER_LANGUAGE C)
target_include_directories(SQLite3 PUBLIC ${SQLite3_DIR})

add_subdirectory(src/modules)
add_subdirectory(src/client)
add_subdirectory(src/test)

add_custom_target(copy-root ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/root
        ${CMAKE_BINARY_DIR}/root
)
