/**
 * Author:    Andrea Casalino
 * Created:   16.05.2019
*
* report any bug to andrecasa91@gmail.com.
 **/

#include <Client.h>
#include <iostream>
using namespace std;

int main(int argc, char **argv){

    cout << "-----------------------  Client  -----------------------" << endl;

    string server_address;
    if(argc > 1) {
       server_address = argv[1];
    }

    //build and initialize a connection to the server on port 2000
    ssk::Client Connection(server_address , 2000);
    Connection.initialize();

    string message;

    //hit the server with a message
    message = "Hi there!";
    cout << "sending to the server: " << message << endl;
    Connection.Send(message);
    
    //get the server response
    message = Connection.RecvStr();
    cout << "got from the server:   " << message << endl;

    //just to keep the prompt active 
    int k;
    cout << "press any key to close" << endl;
    cin >> k;
    return 0;
}