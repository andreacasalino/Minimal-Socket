/**
 * Author:    Andrea Casalino
 * Created:   16.05.2019
*
* report any bug to andrecasa91@gmail.com.
 **/

#include "../../Utils.h"
using namespace std;

int main(int argc, char **argv){

    cout << "-----------------------  Repeater  -----------------------" << endl;

    if(argc < 2) {
        cout << "you did not pass the server address\n";
        return -1;
    }
    string server_address(argv[1]);

    //init the connection to client
    Stream_to_Client connection_to_client(3000);
    connection_to_client.InitConnection();
    cout << "client connected" << endl;

    //init the connection to server
    Stream_to_Server connection_to_server(server_address, 2000);
    connection_to_server.InitConnection();
    cout << "server connected" << endl;

    int temp;
    while (true){
    //receive from client number to forward to server
        temp = connection_to_client.Recv_int();
        cout << "forwarding to server: " << temp << endl;
        connection_to_server.Send_int(temp);

    //receive from server number to forward to client
        temp = connection_to_server.Recv_int();
        cout << "forwarding to client: " << temp << endl;
        connection_to_client.Send_int(temp);
    }

    return 0;
}