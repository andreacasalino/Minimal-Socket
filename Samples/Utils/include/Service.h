/**
* Author:    Andrea Casalino
* Created:   16.05.2019
*
* report any bug to andrecasa91@gmail.com.
**/

#ifndef _SERVICE_
#define _SERVICE_

#include <StringClient.h>
#include <unordered_map>

void crossSleep(const int& ms);



sck::Address parseAddress(int argc, char **argv, const std::uint16_t& port);



class Names {
public:
    static const std::string& getFirstName();
    static const std::string& getNextName(const std::string& attualName);
    static const std::string& getSurname(const std::string& name);
private:
    static const std::unordered_map<std::string, std::string> names;
};

class Service {
public:
    Service(std::unique_ptr<sck::StringClient> connection) 
        : connection(std::move(connection)) {
        };

    bool serve();

    void serveForever();
private:
    std::unique_ptr<sck::StringClient> connection;
};


void ClientLoop(sck::StringClient& client, const int& sleepTime);

#endif
