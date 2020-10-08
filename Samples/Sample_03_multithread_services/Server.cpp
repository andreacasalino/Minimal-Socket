/**
 * Author:    Andrea Casalino
 * Created:   16.05.2019
*
* report any bug to andrecasa91@gmail.com.
 **/

#include "../Utils.h"
#include <thread>
using namespace std;

int main(){

    cout << "-----------------------  Server  -----------------------" << endl;

    //init connection to client A
    IncrementService service_clientA(2000);
    //spawn a thread serving client A
    std::thread threadA([&service_clientA]() { service_clientA.serveForever(); });
    cout << "service to client A spawned" << endl;

    //init connection to client B
    IncrementService service_clientB(3000);
    //spawn a thread serving client B
    std::thread threadB([&service_clientB]() { service_clientB.serveForever(); });
    cout << "service to client B spawned" << endl;

    threadA.join();
    threadB.join();

    return 0;
}