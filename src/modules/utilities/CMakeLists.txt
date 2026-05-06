add_library(utilities STATIC
    adaptors/KeyValueAdaptor.cpp
    adaptors/KeyValueAdaptor.hpp
    adaptors/ParamPathAdaptor.cpp
    adaptors/ParamPathAdaptor.hpp)

target_include_directories(utilities PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
