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

    sck::Address remoteAddress = parseAddress(argc, argv, 31000);
    cout << "Asking connection to " << remoteAddress.getHost() << ":" << remoteAddress.getPort() << endl;

    //build and initialize a connection to the server
    sck::StringClient client( std::make_unique<sck::TcpClient>(remoteAddress) );
    client.open();

    ClientLoop(client , 100);
    
    return EXIT_SUCCESS;
}