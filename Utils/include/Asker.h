/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef SAMPLE_ASKER_H
#define SAMPLE_ASKER_H

#include <core/Client.h>
#include <Names.h>

class Asker {
public:
    Asker(std::unique_ptr<sck::Client> socket);

    void ask();

    void askForever(const std::chrono::milliseconds& sampleTime);

private:
    std::unique_ptr<sck::Client> socket;
    Names cursor;
    char recvBuffer[1000];
};

#endif