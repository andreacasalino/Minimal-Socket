/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef SAMPLE_RESPONDER_ASYNC_H
#define SAMPLE_RESPONDER_ASYNC_H

#ifdef ASYNCH_ENABLED
#include <client/AsyncClient.h>
#include <Names.h>

class ResponderAsync
    : public sck::async::MessageListener
    , public sck::async::ErrorListener {
public:
    ResponderAsync(std::unique_ptr<sck::Client> socket);

    inline bool isRunning() const { return this->running; };

private:
    void handle(const std::pair<const char*, std::size_t>& message) final;

    void handle(const sck::Error& error) final;

    void handle(const std::exception& error) final;

    std::unique_ptr<sck::async::AsyncClient> asyncSocket;
    std::atomic_bool running;
};
#endif

#endif