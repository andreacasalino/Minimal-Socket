/**
 * Author:    Andrea Casalino
 * Created:   16.05.2019
*
* report any bug to andrecasa91@gmail.com.
 **/

#include <Service.h>
#include <TcpServer.h>
#include <iostream>
using namespace std;

int main(){

    cout << "-----------------------  Server  -----------------------" << endl;

    // build and initialize a connection from a client on port 2000
    sck::TcpServer server(2000);
    server.open();

    //accept the client and create the service
    Service srv(std::make_unique<sck::StringClient>(server.acceptNewClient()));

    srv.serveForever();

    return EXIT_SUCCESS;
}