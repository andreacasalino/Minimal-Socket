/**
 * Author:    Andrea Casalino
 * Created:   16.05.2019
*
* report any bug to andrecasa91@gmail.com.
 **/

#include "../../Utils.h"
using namespace std;

int main(int argc, char **argv){

    cout << "-----------------------  Client A, port 2000  -----------------------" << endl;

    if(argc < 2) {
        cout << "you did not pass the server address\n";
        return -1;
    }
    string server_address(argv[1]);

    Client_loop(server_address, 2000 , 10, 0);

    return 0;
}