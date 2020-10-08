/**
 * Author:    Andrea Casalino
 * Created:   16.05.2019
*
* report any bug to andrecasa91@gmail.com.
 **/

#include "../Utils.h"
using namespace std;

int main(int argc, char **argv){

    cout << "-----------------------  Client  -----------------------" << endl;

    string repeater_address;
    if (argc > 1) {
       repeater_address = argv[1];
    }

    ClientLoop(repeater_address, 3000 , 10, 200);

    return 0;
}