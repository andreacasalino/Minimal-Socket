/**
 * Author:    Andrea Casalino
 * Created:   16.05.2019
*
* report any bug to andrecasa91@gmail.com.
 **/

#include "../../../src/Stream_Socket.h"
#include <iostream>
using namespace std;

int main(int argc, char **argv){

    cout << "-----------------------  Client  -----------------------" << endl;

    if(argc < 2) {
        cout << "you did not pass the server address\n";
        return -1;
    }
    string server_address(argv[1]);

    //build and initialize a connection to the server on port 2000
    Stream_to_Server Connection(server_address , 2000);
    Connection.InitConnection();

    string message;

    //hit the server with a message
    message = "Hi there!";
    cout << "sending to the server: " << message << endl;
    Connection.Send_str(message);
    
    //get the server response
    message = Connection.Recv_str();
    cout << "got from the server:   " << message << endl;

    return 0;
}