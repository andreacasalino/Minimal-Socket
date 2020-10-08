/**
 * Author:    Andrea Casalino
 * Created:   16.05.2019
*
* report any bug to andrecasa91@gmail.com.
 **/

#include "../Utils.h"
using namespace std;

int main(int argc, char **argv){

    cout << "-----------------------  Repeater  -----------------------" << endl;

    string server_address;
    if (argc > 1) {
       server_address = argv[1];
    }

    //init the connection to client
    ssk::Server connection_to_client(3000);
    connection_to_client.initialize();
    cout << "client connected" << endl;

    //init the connection to server
    ssk::Client connection_to_server(server_address, 2000);
    connection_to_server.initialize();
    cout << "server connected" << endl;

    int temp;
    while (true){
    //receive from client number to forward to server
        temp = connection_to_client.RecvInt();
        cout << "forwarding to server: " << temp << endl;
        connection_to_server.Send(temp);

    //receive from server number to forward to client
        temp = connection_to_server.RecvInt();
        cout << "forwarding to client: " << temp << endl;
        connection_to_client.Send(temp);
    }

    return 0;
}