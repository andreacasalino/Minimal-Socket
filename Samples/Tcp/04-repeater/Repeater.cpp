/**
 * Author:    Andrea Casalino
 * Created:   16.05.2019
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <tcp/TcpServer.h>
#include <tcp/TcpClient.h>
#include <iostream>
using namespace std;

int main(int argc, char **argv){

    cout << "-----------------------  Repeater  -----------------------" << endl;

    // connecting to the server
    std::unique_ptr<sck::tcp::TcpClient> connection2Server = std::make_unique<sck::tcp::TcpClient>(*sck::Ip::createLocalHost(3000));
    cout << "Asking connection to " << connection2Server->getRemoteAddress().getHost() << ":" << connection2Server->getRemoteAddress().getPort() << endl;

    // accepting client
    std::unique_ptr<sck::Client> connection2Client;
    {
        sck::tcp::TcpServer server(3000);
        server.open(std::chrono::milliseconds(0));
        if (!server.isOpen()) {
            cout << "server open failed" << endl;
            return EXIT_FAILURE;
        }
        connection2Client = server.acceptClient();
        cout << "client connected" << endl;
    }

    char buffer[1000];
    std::pair<char*, std::size_t> temp = { &buffer[0], 1000 };
    std::size_t recvBytes;
    while (true) {
        recvBytes = connection2Client->receive(temp, std::chrono::milliseconds(0));
        cout << "forwarding to server " << std::string(buffer, recvBytes);
        connection2Server->send({&buffer[0], recvBytes});

        recvBytes = connection2Client->receive(temp, std::chrono::milliseconds(0));
        cout << " reply to client " << std::string(buffer, recvBytes) << endl;
        connection2Client->send({ &buffer[0], recvBytes });
    }

    return EXIT_SUCCESS;
}