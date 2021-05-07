/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef SAMPLE_ASYNCRESPONDER_H
#define SAMPLE_ASYNCRESPONDER_H

#ifdef ASYNCH_ENABLED
#include <messanger/AsyncMessanger.h>
#include <NamesMap.h>
#include <Logger.h>

namespace sck::sample {
    class AsyncResponder
        : public sck::async::AsyncMessanger
        , protected sck::async::MessangerListener
        , protected sck::async::ErrorListener
        , public Logger {
    public:
        AsyncResponder(std::unique_ptr<sck::Socket> socket);

    private:
        void handle(const std::pair<const char*, std::size_t>& message) final;

        void handle(const sck::Error& error) final;

        void handle(const std::exception& error) final;
    };
}
#endif

#endif