/**
 * Author:    Andrea Casalino
 * Created:   16.05.2019
*
* report any bug to andrecasa91@gmail.com.
 **/

#include "../../../src/Stream_Socket.h"
#include <iostream>
using namespace std;

int main(){

    cout << "-----------------------  Server  -----------------------" << endl;

    //build and initialize a connection from a client on port 2000
    Stream_to_Client Connection(2000);
    Connection.InitConnection();

    string message;

    //listen to the client request
    message = Connection.Recv_str();
    cout << "got from the client: " << message << endl;
    
    //send a response
    message = "You successfully hit the server";
    cout << "sending response to the client:   " << message << endl;
    Connection.Send_str(message);

    return 0;
}