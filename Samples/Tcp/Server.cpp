/**
 * Author:    Andrea Casalino
 * Created:   16.05.2019
*
* report any bug to andrecasa91@gmail.com.
 **/

#include <Responder.h>
#include <tcp/TcpServer.h>
#include <iostream>
#include <thread>
#include <list>
using namespace std;

class AcceptedClient {
public:
    AcceptedClient(std::unique_ptr<sck::Client> acceptedClient) : acceptedClient(std::move(acceptedClient)) {};

    void run() {
        Responder resp(std::move(this->acceptedClient));
        resp.respondForever();
    };

private:
    std::unique_ptr<sck::Client> acceptedClient;
};

int main(int argc, char** argv){
    cout << "-----------------------  Server  -----------------------" << endl;

    if (argc == 1) {
        cout << "correct syntax is: 'port to bind', 'number of clients to accept'" << endl;
        return EXIT_FAILURE;
    }

    std::uint16_t port = std::atoi(argv[1]);
    std::size_t clientNumbers = 1;
    if (argc > 2) {
        clientNumbers = std::atoi(argv[2]);
    }
    if (0 == clientNumbers) {
        cout << "invalid number of clients" << endl;
        return EXIT_FAILURE;
    }
    cout << "clients excepted " << clientNumbers << endl;

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

    std::list<AcceptedClient> clients;
    std::list<std::thread> clientThreads;
    for (std::size_t c = 0; c < clientNumbers; ++c) {
        //accept the client
        clients.emplace_back(server.acceptClient());
        cout << "new client connected" << endl;
        clientThreads.emplace_back(&AcceptedClient::run, &clients.back());
    }

    for (auto it = clientThreads.begin(); it != clientThreads.end(); ++it) {
        it->join();
    }

    return EXIT_SUCCESS;
}