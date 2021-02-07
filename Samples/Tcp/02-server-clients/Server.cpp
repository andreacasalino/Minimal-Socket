/**
 * Author:    Andrea Casalino
 * Created:   16.05.2019
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <Responder.h>
#include <tcp/TcpServer.h>
#include <iostream>
using namespace std;

int main() {

    cout << "-----------------------  Server  -----------------------" << endl;

    // build and initialize a connection from a client on port 20000
    sck::tcp::TcpServer server(31000);

    // blocking open
    server.open(std::chrono::milliseconds(0));
    if (!server.isOpen()) {
        cout << "server open failed" << endl;
        return EXIT_FAILURE;
    }

    //accept first client
    auto clientAHandler = server.acceptClient();
    cout << "client A connected" << endl;

    //accept second client
    auto clientBHandler = server.acceptClient();
    cout << "client B connected" << endl;

    Responder respA(std::move(clientAHandler));
    Responder respB(std::move(clientBHandler));

    while (true) {
        respA.respond();
        respB.respond();
    }

    return EXIT_SUCCESS;
}