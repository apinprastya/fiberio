#include <fiberio/all.hpp>
#include <boost/fiber/all.hpp>
#include <iostream>

namespace fibers = boost::fibers;
namespace this_fiber = boost::this_fiber;

void uv_sleep_callback(uv_timer_t* handle)
{
    std::cout << "uv_sleep_callback\n";
    void* data = uv_handle_get_data((uv_handle_t*) handle);
    fibers::promise<void>* promise = static_cast<fibers::promise<void>*>(data);
    promise->set_value();
    std::cout << "returning from uv_sleep_callback\n";
}

void handle_client(std::unique_ptr<fiberio::tcpsocket> client)
{
    std::cout << "handle_client\n";
    while(true) {
        std::string data = client->read();
        if (data == "close\r\n") {
            std::cout << "closing connection\n";
            client->write("ok. closing.\n");
            return;
        } else {
            std::cout << "read " << data.size() << " bytes\n";
            if (data.empty()) break;
            std::cout << "echoing data: " << data;
            client->write(data);
        }
    }
}

int main()
{
    fiberio::use_on_this_thread();

    std::cout << "sleep_for(100)\n";
    this_fiber::sleep_for(std::chrono::milliseconds(100));
    std::cout << "returned to main\n";

    auto socket{ fiberio::tcpsocket::create() };

    auto addr = fiberio::getaddrinfo("127.0.0.1", "5500");
    std::cout << "got addrinfo\n";

    std::cout << "binding socket\n";
    socket->bind(addr);

    std::cout << "listening on socket\n";
    socket->listen(50);

    while(true) {
        std::cout << "waiting for client...\n";
        auto client = socket->accept();
        std::cout << "accepted client!\n";
        fibers::async(handle_client, std::move(client));
    }

    std::cout << "exiting main()\n";
    return 0;
}