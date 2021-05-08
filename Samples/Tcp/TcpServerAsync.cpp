/**
 * Author:    Andrea Casalino
 * Created:   16.05.2019
*
* report any bug to andrecasa91@gmail.com.
 **/

#include <AsyncResponder.h>
#include <tcp/TcpServer.h>
#include <iostream>
#include <list>
using namespace std;

int main(int argc, char** argv) {
    cout << "-----------------------  Server Async  -----------------------" << endl;

    if (argc == 1) {
        cout << "correct syntax is: 'port to bind', 'number of clients to accept'" << endl;
        return EXIT_FAILURE;
    }

    std::uint16_t port = std::atoi(argv[1]);
    std::size_t clientNumbers = 1;
    if (argc > 2) {
        clientNumbers = std::atoi(argv[2]);
    }
    if (0 == clientNumbers) {
        cout << "invalid number of clients" << endl;
        return EXIT_FAILURE;
    }
    cout << "clients excepted " << clientNumbers << endl;

    //build the server object
    sck::tcp::TcpServer server(port);
    cout << "Binding port " << port << endl;

    // blocking open
    server.open(std::chrono::milliseconds(0));
    if (!server.isOpen()) {
        cout << "server open failed" << endl;
        return EXIT_FAILURE;
    }
    cout << "connection opened" << endl;

    std::list<sck::sample::AsyncResponder> responders;
    for (std::size_t c = 0; c < clientNumbers; ++c) {
        //accept the client
        cout << "waiting client " << c << endl;
        responders.emplace_back(server.acceptClient());
        cout << "new client connected" << endl;
        responders.back().open(std::chrono::milliseconds(0));
    }

    while (!responders.empty()) {
        auto it = responders.begin();
        while (it != responders.end()) {
            if (!it->isOpen()) {
                it = responders.erase(it);
            }
            else ++it;
        }
    }

    return EXIT_SUCCESS;
}