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
    cout << "-----------------------  Client Asker -----------------------" << endl;

    if (argc == 1) {
        cout << "correct syntax is: 'server port', <optional>'rate', <optional>'port to reserve',  <optional>'send initial handshake flag 0/1', <optional>'server host'" << endl;
        return EXIT_FAILURE;
    }

    std::uint16_t serverPort = std::atoi(argv[1]);

    std::chrono::milliseconds rate(200);
    if (argc > 2) {
        rate = std::chrono::milliseconds(std::atoi(argv[2]));
    }
    cout << "rate assumed " << rate.count() << " [ms]" << endl;

    std::uint16_t port = 0;
    if (argc > 3) {
        port = std::atoi(argv[3]);
    }
    cout << "port reserved by this udp " << port << endl;

    bool handShake = false;
    if (argc > 4) {
        handShake = static_cast<bool>(std::atoi(argv[4]));
    }

    sck::IpPtr serverAddress;
    if (argc > 5) {
        serverAddress = sck::Ip::create(std::string(argv[5]), serverPort);
    }
    else {
        serverAddress = sck::Ip::createLocalHost(serverPort);
    }
    if (nullptr == serverAddress) {
        cout << "invalid server address" << endl;
        return EXIT_FAILURE;
    }

    std::unique_ptr<sck::udp::UdpClient> client = std::make_unique<sck::udp::UdpClient>(*serverAddress, port);
    cout << "Expecting connection to " << client->getRemoteAddress().getHost() << ":" << client->getRemoteAddress().getPort() << endl;

    // blocking open
    client->open(std::chrono::milliseconds(0));
    cout << "connection opened" << endl;

    if (handShake) {
        std::string hello = "hello";
        client->send({hello.data(), hello.size()});
        cout << "handshake sent" << endl;
    }

    Asker ask(std::move(client));
    ask.askForever(rate);
    
    return EXIT_SUCCESS;
}