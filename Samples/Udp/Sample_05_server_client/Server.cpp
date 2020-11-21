/**
 * Author:    Andrea Casalino
 * Created:   16.05.2019
*
* report any bug to andrecasa91@gmail.com.
 **/

#include <Service.h>
#include <UdpClient.h>
#include <iostream>
using namespace std;

int main(int argc, char **argv){

    cout << "-----------------------  Server  -----------------------" << endl;

    sck::Address clientAddress = parseAddress(argc, argv, 3000);
    cout << "Client expected at " << clientAddress.getHost() << ":" << clientAddress.getPort() << endl;

    //build and initialize a connection to the client on port 3000 and reserve port 2000
    std::unique_ptr<sck::StringClient> server = std::make_unique<sck::StringClient>(std::make_unique<sck::UdpClient>(clientAddress, 2000));
    server->open();
    Service srv(std::move(server));

    srv.serveForever();

    return EXIT_SUCCESS;
}