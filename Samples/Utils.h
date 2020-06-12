/**
 * Author:    Andrea Casalino
 * Created:   16.05.2019
*
* report any bug to andrecasa91@gmail.com.
 **/

#include "../src/Stream_Socket.h"
#include <iostream>


#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
void __Sleep__(const int& sleep_ms){
#ifdef _WIN32
    Sleep(sleep_ms);
#else
    usleep(sleep_ms * 1000);
#endif
}



class Increment_service{
public:
    Increment_service(const int& service_port) : Connection(service_port){
        this->Connection.InitConnection();
    }

    void serve(){
        int to_increment = this->Connection.Recv_int();
        this->Connection.Send_int(to_increment + 1);
    };

    void serve_forever(){
        bool life = true;
        while (life){
            try{
                this->serve();
            }
            catch(const std::exception& e) {
                //connection close
                life = false;
            }
        }
    }

    //used by pthread
    static void* serve_forever(void* obj){
        Increment_service* pt = static_cast<Increment_service*>(obj);
        pt->serve_forever();
        return NULL;
    }
private:
// data
    Stream_to_Client Connection;
};


void Client_loop(const std::string& server_address, const int& server_port, const int& number_max, const int& sleep_time){    
    //build and initialize a connection to the server
    std::cout << "attempting connection to " << server_address << ":" << server_port << std::endl;
    Stream_to_Server Connection(server_address , server_port);
    Connection.InitConnection();
    std::cout << "connected" << std::endl;

    int number = 0;
//keep sending request to increment the number to the server
    while (true){
        Connection.Send_int(number);
        std::cout << "sending: " << number;

        number = Connection.Recv_int();
        //the number should have been increased by the server
        std::cout << "   got from the server: " << number << std::endl;

        if(number == number_max) number = 0;

        if(sleep_time != 0) __Sleep__(sleep_time);
    }
}