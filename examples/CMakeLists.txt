
add_executable(app fiber_echo_server.cpp)

target_link_libraries(app PRIVATE fiberio)

#add_executable(benchmark fiber_benchmarks.cpp)

#target_link_libraries(benchmark PRIVATE fiberio)