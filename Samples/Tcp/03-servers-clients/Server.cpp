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
using namespace std;

int main(){

    cout << "-----------------------  Server  -----------------------" << endl;

    //build two services: one for client A and one for client B
    sck::tcp::TcpServer server(27300);

    // blocking open
    server.open(std::chrono::milliseconds(0));
    if (!server.isOpen()) {
        cout << "server open failed" << endl;
        return EXIT_FAILURE;
    }

    auto srv = [&server](){
        //accept the client
        auto clientHandler = server.acceptClient();
        cout << "client connected" << endl;

        Responder resp(std::move(clientHandler));
        resp.respondForever();
    };

    //spawn service to client A
    std::thread tA(srv);
    //spawn service to client B
    std::thread tB(srv);

    tA.join();
    tB.join();

    return EXIT_SUCCESS;
}