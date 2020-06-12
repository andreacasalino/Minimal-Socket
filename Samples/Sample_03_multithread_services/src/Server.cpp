/**
 * Author:    Andrea Casalino
 * Created:   16.05.2019
*
* report any bug to andrecasa91@gmail.com.
 **/

#include "../../Utils.h"
#include <pthread.h>
using namespace std;

int main(){

    cout << "-----------------------  Server  -----------------------" << endl;

    //init connection to client A
    Increment_service service_clientA(2000);
    //spawn a thread serving client A
    pthread_t threadA;
    if(pthread_create(&threadA ,NULL, &Increment_service::serve_forever, &service_clientA)){
        return -1;
        system("echo unable to create thread");
    }
    cout << "service to client A spawned" << endl;

    //init connection to client B
    Increment_service service_clientB(3000);
    //spawn a thread serving client B
    pthread_t threadB;
    if(pthread_create(&threadB ,NULL, &Increment_service::serve_forever, &service_clientB)){
        return -1;
        system("echo unable to create thread");
    }
    cout << "service to client B spawned" << endl;


    void *status;
    pthread_join(threadA, &status);
    pthread_join(threadB, &status);

    return 0;
}