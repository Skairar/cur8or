add_library(client-network STATIC
    SocketListener.cpp
    SocketListener.hpp
    HTTP/SimpleSession.cpp
    HTTP/SimpleSession.hpp
    HTTP/MessageProcessor.cpp
    HTTP/MessageProcessor.hpp
    HTTP/MessageProcessor.tpp
    HTTP/ResponseTemplate.cpp
    HTTP/ResponseTemplate.hpp
    HTTP/ResponseFinalizer.hpp
    HTTP/ResponseFinalizer.cpp
    HTTP/helpers/FileMimeType.cpp
    HTTP/helpers/FileMimeType.hpp)

target_include_directories(client-network PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)

# Boost.Asio/Beast are header-only and part of the public interface
target_link_libraries(client-network PUBLIC Boost::headers)

if(WIN32)
    target_link_libraries(client-network PRIVATE ws2_32 wsock32)
endif()
