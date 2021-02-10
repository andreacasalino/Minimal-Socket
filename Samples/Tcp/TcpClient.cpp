/**
 * Author:    Andrea Casalino
 * Created:   16.05.2019
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <Asker.h>
#include <tcp/TcpClient.h>
#include <iostream>
using namespace std;

int main(int argc, char **argv){
    cout << "-----------------------  Client  -----------------------" << endl;

    if (argc == 1) {
        cout << "correct syntax is: 'server port', <optional>'rate', <optional>'server host'" << endl;
        return EXIT_FAILURE;
    }

    std::uint16_t serverPort = std::atoi(argv[1]);

    std::chrono::milliseconds rate(200);
    if (argc > 2) {
        rate = std::chrono::milliseconds(std::atoi(argv[2]));
    }
    cout << "rate assumed " << rate.count() << " [ms]" << endl;

    sck::IpPtr serverAddress;
    if (argc > 3) {
        serverAddress = sck::Ip::create(std::string(argv[3]), serverPort);
    }
    else {
        serverAddress = sck::Ip::createLocalHost(serverPort);
    }
    if (nullptr == serverAddress) {
        cout << "invalid server address" << endl;
        return EXIT_FAILURE;
    }

    std::unique_ptr<sck::tcp::TcpClient> client = std::make_unique<sck::tcp::TcpClient>(*serverAddress);
    cout << "Asking connection to " << client->getRemoteAddress().getHost() << ":" << client->getRemoteAddress().getPort() << endl;

    // blocking open
    client->open(std::chrono::milliseconds(0));
    if (!client->isOpen()) {
        cout << "connection failed" << endl;
        return EXIT_FAILURE;
    }
    cout << "connection opened" << endl;

    Asker ask(std::move(client));
    ask.askForever(rate);
    
    return EXIT_SUCCESS;
}