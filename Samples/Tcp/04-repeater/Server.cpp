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
    sck::tcp::TcpServer server(3000);

    // blocking open
    server.open(std::chrono::milliseconds(0));
    if (!server.isOpen()) {
        cout << "server open failed" << endl;
        return EXIT_FAILURE;
    }

    //accept the client
    auto clientHandler = server.acceptClient();
    cout << "client connected" << endl;

    Responder resp(std::move(clientHandler));
    resp.respondForever();

    return EXIT_SUCCESS;
}