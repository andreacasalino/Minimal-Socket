/**
 * Author:    Andrea Casalino
 * Created:   16.05.2019
*
* report any bug to andrecasa91@gmail.com.
 **/

#include "../Utils.h"
using namespace std;

int main(int argc, char **argv){

    cout << "-----------------------  Client A, port 2000  -----------------------" << endl;

    string server_address;
    if (argc > 1) {
       server_address = argv[1];
    }

    ClientLoop(server_address, 2000 , 10, 500); //500ms of sleep for next iteration

    return 0;
}