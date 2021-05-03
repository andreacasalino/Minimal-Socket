/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef SAMPLE_ASKER_H
#define SAMPLE_ASKER_H

#include <core/Messanger.h>
#include <Names.h>
#include <Logger.h>

namespace sck::sample {
    class Asker
        : public Logger {
    public:
        Asker(std::unique_ptr<sck::Messanger> socket);

        void ask();

        void askForever(const std::chrono::milliseconds& sampleTime);

    private:
        std::unique_ptr<sck::Messanger> socket;
        Names cursor;
        char recvBuffer[1000];
    };
}

#endif