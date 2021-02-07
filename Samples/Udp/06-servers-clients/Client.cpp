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

int main(int argc, char** argv) {

    cout << "-----------------------  Client  -----------------------" << endl;
    
    if (argc < 2) {
        cout << "sleep time not passed" << endl;
        return EXIT_FAILURE;
    }
    int sleepTime = atoi(std::string(argv[1]).c_str());
    
    if (argc < 3) {
        cout << "port to bind not passed" << endl;
        return EXIT_FAILURE;
    }
    std::unique_ptr<sck::udp::UdpClient> client = std::make_unique<sck::udp::UdpClient>(*sck::Ip::createLocalHost(31000), std::atoi(argv[2]));

    // blocking open
    client->open(std::chrono::milliseconds(0));
    if (!client->isOpen()) {
        cout << "connection failed" << endl;
        return EXIT_FAILURE;
    }

    Asker ask(std::move(client));
    ask.askForever(std::chrono::milliseconds(sleepTime));

    return EXIT_SUCCESS;
}