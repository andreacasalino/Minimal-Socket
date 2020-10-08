/**
 * Author:    Andrea Casalino
 * Created:   16.05.2019
*
* report any bug to andrecasa91@gmail.com.
 **/

#include <Server.h>
#include <Client.h>
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



class IncrementService{
public:
    IncrementService(const int& service_port) : Connection(service_port){ this->Connection.initialize(); }

    void serve(){
        int to_increment = this->Connection.RecvInt();
        this->Connection.Send(to_increment + 1);
    };

    void serveForever(){
        bool life = true;
        while (life){
            try{
                this->serve();
            }
            catch(...) {
                //connection close
                life = false;
            }
        }
    }

private:
// data
    ssk::Server Connection;
};


void ClientLoop(const std::string& server_address, const int& server_port, const int& number_max, const int& sleep_time){    
    //build and initialize a connection to the server
    std::cout << "attempting connection to " << server_address << ":" << server_port << std::endl;
    ssk::Client Connection(server_address , server_port);
    Connection.initialize();
    std::cout << "connected" << std::endl;

    int number = 0;
//keep sending request to increment the number to the server
    while (true){
        Connection.Send(number);
        std::cout << "sending: " << number;

        number = Connection.RecvInt();
        //the number should have been increased by the server
        std::cout << "   got from the server: " << number << std::endl;

        if(number == number_max) number = 0;

        if(sleep_time != 0) __Sleep__(sleep_time);
    }
}