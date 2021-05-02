/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef SAMPLE_RESPONDER_H
#define SAMPLE_RESPONDER_H

#include <core/Client.h>
#include <Names.h>

class Responder {
public:
    Responder(std::unique_ptr<sck::Client> socket);

    void respond();

    void respondForever();

private:
    std::unique_ptr<sck::Client> socket;
    char recvBuffer[1000];
};

#endif