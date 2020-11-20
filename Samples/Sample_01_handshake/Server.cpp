/**
 * Author:    Andrea Casalino
 * Created:   16.05.2019
*
* report any bug to andrecasa91@gmail.com.
 **/

#include <TcpServer.h>
#include <StringClient.h>
#include <iostream>
using namespace std;

int main(){

    cout << "-----------------------  Server  -----------------------" << endl;

    // build and initialize a connection from a client on port 2000
    sck::TcpServer server(2000);
    server.open();

    //accept the client
    sck::StringClient clientHandle(server.acceptNewClient());

    string message;

    //listen to the client request
    message = *clientHandle.receive(500).get();
    cout << "got from the client: " << message << endl;
    
    //send a response
    message = "You successfully hit the server";
    cout << "sending response to the client:   " << message << endl;
    clientHandle.send(message);

    std::unique_ptr<sck::TcpServer> lklk;

    return 0;
}