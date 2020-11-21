/**
 * Author:    Andrea Casalino
 * Created:   16.05.2019
*
* report any bug to andrecasa91@gmail.com.
 **/

#include <StringClient.h>
#include <TcpClient.h>
#include <TcpServer.h>
#include <iostream>
using namespace std;

int main(int argc, char **argv){

    cout << "-----------------------  Repeater  -----------------------" << endl;

    // connecting to the server
    sck::StringClient connection2Server( std::make_unique<sck::TcpClient>(sck::Address::Localhost(3000)) );
    connection2Server.open();
    cout << "connected to server" << endl;

    // accepting client
    std::unique_ptr<sck::StringClient> connection2Client;
    {
        sck::TcpServer server(4000);
        server.open();
        connection2Client = std::make_unique<sck::StringClient>(server.acceptNewClient());
    }

    while (true) {
        auto toForward = connection2Client->receive(500);
        if(toForward->size() == 0) {
            connection2Server.send("");
            break;
        }
        cout << "forwarding to server " << *toForward.get();
        connection2Server.send(*toForward.get());

        auto reply = connection2Server.receive(500);
        cout << " reply to client " << *reply.get() << endl;
        connection2Client->send(*reply.get());
    }

    return EXIT_SUCCESS;
}