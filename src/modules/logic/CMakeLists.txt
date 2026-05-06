add_library(client-logic STATIC
    PathResolver/VirtualPathResolver.cpp
    PathResolver/VirtualPathResolver.hpp
    PathResolver/Query.cpp
    PathResolver/Query.hpp)

target_include_directories(client-logic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_link_libraries(client-logic PRIVATE utilities)
