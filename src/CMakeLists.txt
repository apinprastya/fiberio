
add_library(fiberio 
    fiberio.cpp
    server_socket.cpp
    server_socket_impl.cpp
    socket.cpp
    socket_impl.cpp
    addrinfo.cpp
    loop.cpp
    scheduler.cpp
    utils.cpp
)

IF (WIN32)
    target_link_libraries(fiberio PRIVATE ${Boost_LIBRARIES} unofficial::libuv::libuv ws2_32)
ELSE()
    target_link_libraries(fiberio PRIVATE ${Boost_LIBRARIES} unofficial::libuv::libuv)
ENDIF()

target_compile_features(fiberio PUBLIC cxx_std_17)