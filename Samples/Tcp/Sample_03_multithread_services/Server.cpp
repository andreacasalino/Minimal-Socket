/**
 * Author:    Andrea Casalino
 * Created:   16.05.2019
*
* report any bug to andrecasa91@gmail.com.
 **/

#include "Service.h"
#include <TcpServer.h>
#include <iostream>
#include <thread>
using namespace std;

int main(){

    cout << "-----------------------  Server  -----------------------" << endl;

    //build two services: one for client A and one for client B
    sck::TcpServer server(27300);
    server.open();

    auto srv = [&](){
        Service service(std::make_unique<sck::StringClient>(server.acceptNewClient()));
        service.serveForever();
    };

    //spawn service to client A
    std::thread tA(srv);
    //spawn service to client B
    std::thread tB(srv);

    tA.join();
    tB.join();

    return 0;
}