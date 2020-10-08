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

    //build two services: one for client A and one for client B

    //init connection to client A
    IncrementService service_clientA(2000);
    cout << "client A connected" << endl;

    //init connection to client B
    IncrementService service_clientB(3000);
    cout << "client B connected" << endl;

    while (true){
    //serve the clients
        service_clientA.serve();
        service_clientB.serve();

    //wait a little bit
        __Sleep__(100);
    }

    return 0;
}