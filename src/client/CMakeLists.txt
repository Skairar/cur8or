add_executable(curator-client main.cpp)

target_link_libraries(curator-client PRIVATE
    client-network
    client-logic
    client-database)

if(MINGW)
    target_link_options(curator-client PRIVATE
        -static-libgcc
        -static-libstdc++
        -Wl,-Bstatic,--whole-archive,-lwinpthread,--no-whole-archive,-Bdynamic)
endif()
