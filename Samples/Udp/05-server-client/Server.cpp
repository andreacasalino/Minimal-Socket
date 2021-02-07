/**
 * Author:    Andrea Casalino
 * Created:   16.05.2019
*
* report any bug to andrecasa91@gmail.com.
 **/

#include <Responder.h>
#include <udp/UdpServer.h>
#include <iostream>
using namespace std;

int main(){

    cout << "-----------------------  Server  -----------------------" << endl;

    // build and initialize a connection from a client on port 20000
    std::unique_ptr<sck::udp::UdpServer> server = std::make_unique<sck::udp::UdpServer>(2000);

    // blocking open: wait for handshake message from client
    server->open(std::chrono::milliseconds(0));
    if (!server->isOpen()) {
        cout << "server open failed" << endl;
        return EXIT_FAILURE;
    }

    Responder resp(std::move(server));
    resp.respondForever();

    return EXIT_SUCCESS;
}