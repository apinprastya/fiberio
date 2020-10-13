#include <fiberio/all.hpp>
#include <boost/fiber/all.hpp>
#include <iostream>
#include <cstring>

namespace fibers = boost::fibers;

#define RESPONSE "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 14\r\n\r\nHello, World!\n"

int main()
{
    fiberio::use_on_this_thread();

    fiberio::server_socket server;
    server.bind("0.0.0.0", 5531);
    server.listen(50);

    while (true)
    {
        fibers::async([](fiberio::socket client) {
            char buf[4096];
            while (client.is_open())
            {
                std::size_t bytes_read{client.read(buf, sizeof(buf))};
                //std::cout << boost::this_fiber::get_id() << std::endl;
                client.write(RESPONSE, strlen(RESPONSE));
                client.close();
            }
        },
                      server.accept());
    }

    return 0;
}
