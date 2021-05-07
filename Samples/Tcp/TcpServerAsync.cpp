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

int main(int argc, char** argv){
    cout << "-----------------------  Server Async  -----------------------" << endl;

    if (argc == 1) {
        cout << "correct syntax is: 'port to bind'" << endl;
        return EXIT_FAILURE;
    }

    std::uint16_t port = std::atoi(argv[1]);

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

    auto clientConenction = server.acceptClient();
    cout << "client accepted" << endl;

    sck::sample::AsyncResponder responder(std::move(clientConenction));
    while(responder.isOpen()) {
    }

    return EXIT_SUCCESS;
}