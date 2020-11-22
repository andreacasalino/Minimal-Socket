#include "../include/Service.h"
#include <iostream>
#include <mutex>


#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
void crossSleep(const int& ms){
#ifdef _WIN32
    Sleep(ms);
#else
    usleep(ms * 1000);
#endif
}


sck::Address parseAddress(int argc, char **argv, const std::uint16_t& port){
    if(argc >= 2) return sck::Address::FromIp( argv[1], port);
    else return sck::Address::Localhost(port);
}



const std::unordered_map<std::string, std::string> Names::names = std::unordered_map<std::string, std::string>
{
    {"Luciano", "Pavarotti"},
    {"Gengis", "Khan"},
    {"Giulio", "Cesare"},
    {"Theodor", "Roosvelt"},
    {"Immanuel", "Kant"}
};

const std::string unknownName = "Unknown";

const std::string& Names::getFirstName(){
    return names.begin()->first;
}

const std::string& Names::getNextName(const std::string& attualName){
    auto attualIt = names.find(attualName);
    if(attualIt == names.end()) return unknownName;
    ++attualIt;
    if(attualIt == names.end()) attualIt = names.begin();
    return attualIt->first;
}

const std::string& Names::getSurname(const std::string& name){
    auto attualIt = names.find(name);
    if(attualIt == names.end()) return unknownName;
    return attualIt->second;
}


std::mutex coutMtx;
bool Service::serve(){
    auto got = this->connection->receive(500);
    {
        std::lock_guard<std::mutex> lk(coutMtx);
        std::cout << "got: " << *got.get() << std::endl;
    }
    if(got->size() == 0) return false;
    this->connection->send(Names::getSurname(*got.get()));
    return true;
};

void Service::serveForever(){
    while (true){
        if(!this->serve()) break;
    }
}


void ClientLoop(sck::StringClient& client, const int& sleepTime){
    std::string name = Names::getFirstName();
//keep sending request to remote server
    while (true){
        std::cout << "sending: " << name;
        client.send(name);

        std::string surname = *client.receive(500).get();
        if(surname.size() == 0) break;
        std::cout << "   got from the server: " << surname << std::endl;

        name = Names::getNextName(name);

        crossSleep(sleepTime);
    }
}