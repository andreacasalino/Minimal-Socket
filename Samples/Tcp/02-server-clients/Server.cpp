/**
 * Author:    Andrea Casalino
 * Created:   16.05.2019
*
* report any bug to andrecasa91@gmail.com.
 **/

#include "Service.h"
#include <TcpServer.h>
#include <iostream>
#include <list>
using namespace std;

int main(){

    cout << "-----------------------  Server  -----------------------" << endl;

    //build two services: one for client A and one for client B
    sck::TcpServer server(31000);
    server.open();
    //init connection to client A
    Service serviceA(std::make_unique<sck::StringClient>(server.acceptNewClient()));
    cout << "client A connected" << endl;
    //init connection to client B
    Service serviceB(std::make_unique<sck::StringClient>(server.acceptNewClient()));
    cout << "client B connected" << endl;

    std::list<Service*> active = {&serviceA, &serviceB};
    while (true){
    //serve all the active clients
        auto it = active.begin();
        while (it!=active.end()){
            if(!(*it)->serve()) it = active.erase(it);
            else ++it;
        }
    }

    return 0;
}