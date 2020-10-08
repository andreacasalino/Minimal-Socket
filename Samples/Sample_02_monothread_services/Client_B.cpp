/**
 * Author:    Andrea Casalino
 * Created:   16.05.2019
*
* report any bug to andrecasa91@gmail.com.
 **/

#include "../Utils.h"
using namespace std;


int main(int argc, char **argv){

    cout << "-----------------------  Client B, port 3000  -----------------------" << endl;

    string server_address;
    if (argc > 1) {
       server_address = argv[1];
    }

    ClientLoop(server_address, 3000 , 20, 0);

    return 0;
}