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

int main(int argc, char** argv) {
    cout << "-----------------------  Server  -----------------------" << endl;

    if (argc == 1) {
        cout << "correct syntax is: 'port to reserve'" << endl;
        return EXIT_FAILURE;
    }

    std::uint16_t serverPort = std::atoi(argv[1]);

    std::unique_ptr<sck::udp::UdpServer> server = std::make_unique<sck::udp::UdpServer>(serverPort);
    cout << "waiting for the client" << endl;

    // blocking open
    server->open(std::chrono::milliseconds(0));
    cout << "connection opened" << endl;

    sck::sample::Responder responder(std::move(server));
    responder.respondForever();

    return EXIT_SUCCESS;
}