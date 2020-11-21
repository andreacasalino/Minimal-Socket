/**
 * Author:    Andrea Casalino
 * Created:   16.05.2019
*
* report any bug to andrecasa91@gmail.com.
 **/

#include "Service.h"
#include <UdpServer.h>
#include <iostream>
#include <thread>
using namespace std;

int main(){

    cout << "-----------------------  Server  -----------------------" << endl;

    //build two services: one for client A and one for client B

    auto srv = [&](const std::uint16_t& localPort){
        std::unique_ptr<sck::StringClient> server = std::make_unique<sck::StringClient>(std::make_unique<sck::UdpServer>(localPort));
        server->open();
        return std::make_unique<Service>(std::move(server));
    };

    //spawn service to client A
    auto srvA = srv(3500);
    std::thread tA([&srvA](){ srvA->serveForever(); });
    //spawn service to client B
    auto srvB = srv(4500);
    std::thread tB([&srvB](){ srvB->serveForever(); });

    tA.join();
    tB.join();

    return 0;
}