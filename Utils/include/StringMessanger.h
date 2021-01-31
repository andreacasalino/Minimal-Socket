/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef STRING_MESSANGER_H
#define STRING_MESSANGER_H

#include <SocketClient.h>
#include <string.h>
#include <map>

class PersonRegister {
public:
    static const std::string& getSurname(const std::string& name);

    static const std::map<std::string, std::string> persons;
private:
    static const std::string unknown;
};
const std::string PersonRegister::unknown = "unknown";
const std::map<std::string, std::string> PersonRegister::persons = { 
    {"Benjamin", "Franklin"},
    {"Leonardo", "Da Vinci"},
    {"Napoleone", "Bonaparte"},
    {"Nikola", "Tesla"},
    {"Luciano", "Pavarotti"},
};



class StringMessanger {
public:
    StringMessanger(std::unique_ptr<sck::SocketClient> socket);

    std::string sendReceive(const std::string& name);
    void        receiveSend();

private:
    std::unique_ptr<sck::SocketClient> socket;
    std::vector<char> receiveBuffer;
};

#endif