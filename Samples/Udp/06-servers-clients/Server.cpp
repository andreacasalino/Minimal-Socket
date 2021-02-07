/**
 * Author:    Andrea Casalino
 * Created:   16.05.2019
*
* report any bug to andrecasa91@gmail.com.
 **/

#include <Responder.h>
#include <udp/UdpClient.h>
#include <iostream>
#include <thread>
using namespace std;

int main(int argc, char** argv){

    cout << "-----------------------  Server  -----------------------" << endl;

    //build two services: one for client A and one for client B    
    std::uint16_t portA, portB;
    if (argc < 2) {
        cout << "port A not passed" << endl;
        return EXIT_FAILURE;
    }
    portA = std::atoi(argv[1]);
    if (argc < 3) {
        cout << "port B not passed" << endl;
        return EXIT_FAILURE;
    }
    portB = std::atoi(argv[2]);

    auto srv = [](const std::uint16_t& port){
        //accept the client
        std::unique_ptr<sck::udp::UdpClient> socket = std::make_unique<sck::udp::UdpClient>(*sck::Ip::createLocalHost(port), 31000);
        socket->open(std::chrono::milliseconds(0));

        Responder resp(std::move(socket));
        resp.respondForever();
    };

    //spawn service to client A
    std::thread tA([&srv, &portA](){ srv(portA); });
    //spawn service to client B
    std::thread tB([&srv, &portB](){ srv(portB); });

    tA.join();
    tB.join();

    return EXIT_SUCCESS;
}