/**
 * Author:    Andrea Casalino
 * Created:   16.05.2019
*
* report any bug to andrecasa91@gmail.com.
 **/

#include "../../Utils.h"
using namespace std;

int main(int argc, char **argv){

    cout << "-----------------------  Client  -----------------------" << endl;

    if(argc < 2) {
        cout << "you did not pass the repeater address\n";
        return -1;
    }
    string repeater_address(argv[1]);

    Client_loop(repeater_address, 3000 , 10, 200);

    return 0;
}