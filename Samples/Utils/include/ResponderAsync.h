/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef SAMPLE_RESPONDER_ASYNC_H
#define SAMPLE_RESPONDER_ASYNC_H

#include <async/AsyncClient.h>
#include <Names.h>

class ResponderAsync : public sck::async::listener::MessageListener {
public:
    ResponderAsync(std::unique_ptr<sck::Client> socket);

private:
    void handle(const std::pair<const char*, std::size_t>& message) final;

    std::unique_ptr<sck::async::AsyncClient> asyncSocket;
};

#endif