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

    cout << "-----------------------  Client  -----------------------" << endl;

    if(argc < 3) return EXIT_FAILURE;
    int sleepTime = atoi(std::string(argv[1]).c_str());
    int port = atoi(std::string(argv[2]).c_str());
    sck::Address remoteAddress = sck::Address::Localhost(port);
    if(argc > 3) remoteAddress = sck::Address::FromIp(argv[3], port);
    cout << "Asking connection to " << remoteAddress.getHost() << ":" << remoteAddress.getPort() << endl;

    //build and initialize a connection to the server
    sck::StringClient client( std::make_unique<sck::UdpClient>(remoteAddress) );
    client.open();
    //send 1 byte to make the server recognize the client
    client.send("A");

    ClientLoop(client , sleepTime);
    
    return EXIT_SUCCESS;
}