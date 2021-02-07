/**
 * Author:    Andrea Casalino
 * Created:   16.05.2019
*
* report any bug to andrecasa91@gmail.com.
 **/

#include <Service.h>
#include <TcpClient.h>
#include <iostream>
using namespace std;

int main(int argc, char **argv){

    cout << "-----------------------  Client  -----------------------" << endl;

    if(argc < 2) return EXIT_FAILURE;
    int sleepTime = atoi(std::string(argv[1]).c_str());
    sck::Address remoteAddress = sck::Address::Localhost(27300);
    if(argc > 2) remoteAddress = sck::Address::FromIp(argv[2], 27300);
    cout << "Asking connection to " << remoteAddress.getHost() << ":" << remoteAddress.getPort() << endl;

    //build and initialize a connection to the server
    sck::StringClient client( std::make_unique<sck::TcpClient>(remoteAddress) );
    client.open();

    ClientLoop(client , sleepTime);
    
    return EXIT_SUCCESS;
}