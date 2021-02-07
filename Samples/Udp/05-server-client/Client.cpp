/**
 * Author:    Andrea Casalino
 * Created:   16.05.2019
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <Asker.h>
#include <udp/UdpClient.h>
#include <iostream>
using namespace std;

int main(int argc, char **argv){

    cout << "-----------------------  Client  -----------------------" << endl;

    std::unique_ptr<sck::udp::UdpClient> client = std::make_unique<sck::udp::UdpClient>(*sck::Ip::createLocalHost(20000));
    cout << "Sending handshake message to " << client->getRemoteAddress().getHost() << ":" << client->getRemoteAddress().getPort() << endl;
    {
        std::string handshakeMessage = "hello";
        client->send({handshakeMessage.data(), handshakeMessage.size()});
    }

    // blocking open
    client->open(std::chrono::milliseconds(0));

    Asker ask(std::move(client));
    ask.askForever(std::chrono::milliseconds(500));
    
    return EXIT_SUCCESS;
}