/**
 * Author:    Andrea Casalino
 * Created:   16.05.2019
*
* report any bug to andrecasa91@gmail.com.
 **/

#include "../Utils.h"
using namespace std;

int main(){

    cout << "-----------------------  Server  -----------------------" << endl;

    //init the connection to repeater
    IncrementService service(2000);
    cout << "repeater connected" << endl;

    //serve forever
    service.serveForever();

    return 0;
}