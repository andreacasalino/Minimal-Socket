/**
 * Author:    Andrea Casalino
 * Created:   16.05.2019
 *
 * report any bug to andrecasa91@gmail.com.
 **/

///////////////////////////////////////////////////////////////////////////
//                     Have a look to README.md                          //
///////////////////////////////////////////////////////////////////////////

// elements from the MinimalSocket library
#include <MinimalSocket/tcp/TcpServer.h>

// just a bunch of utilities
#include <Args.h>
#include <Pollables.h>
#include <Respond.h>
#include <TimeOfDay.h>

#include <functional>
#include <list>
using namespace std;

int main(const int argc, const char **argv) {
  cout << "-----------------------  Server  -----------------------" << endl;
  PARSE_ARGS

  const auto server_port = options->getValue<MinimalSocket::Port>("port");
  const auto max_clients = options->getValue<int>("clients", 0);
  const auto family = options->getValue<MinimalSocket::AddressFamily>(
      "family", MinimalSocket::AddressFamily::IP_V4);

  MinimalSocket::tcp::TcpServer<false> server(server_port, family);

  if (!server.open()) {
    cerr << "Failed to bind and listen to specified port" << endl;
    return EXIT_FAILURE;
  }
  cout << "Listening for new clients on port " << server_port << endl;

  std::size_t connected = 0;
  std::list<MinimalSocket::tcp::TcpConnectionNonBlocking> connections;
  MinimalSocket::samples::Pollables pollables;

  auto create_pollable_connection =
      [&](MinimalSocket::tcp::TcpConnectionNonBlocking &&connection) {
        auto &conn = connections.emplace_back(
            std::forward<MinimalSocket::tcp::TcpConnectionNonBlocking>(
                connection));
        return [conn = &conn]() {
          // poll the connection by doing a non blocking receive
          try {
            auto request = conn->receive(500);
            if (request.empty()) {
              return MinimalSocket::samples::PollableStatus::NOT_ADVANCED;
            }
            const auto &response =
                MinimalSocket::samples::NamesCircularIterator::NAMES_SURNAMES
                    .find(request)
                    ->second;
            cout << MinimalSocket::samples::TimeOfDay{}
                 << " received: " << request << " ; sending: " << response
                 << endl;
            conn->send(response);
          } catch (const MinimalSocket::SocketError &) {
            // if here the connection was closed
            return MinimalSocket::samples::PollableStatus::COMPLETED;
          }
          return MinimalSocket::samples::PollableStatus::ADVANCED;
        };
      };

  pollables.emplace([&]() {
    // poll the acceptor by trying to accept a new client
    auto maybe_new_connection = server.acceptNewNonBlockingClient();
    if (maybe_new_connection.has_value()) {
      cout << MinimalSocket::samples::TimeOfDay{}
           << " connected a new client from "
           << MinimalSocket::to_string(maybe_new_connection->getRemoteAddress())
           << endl;
      pollables.emplace(
          create_pollable_connection(std::move(maybe_new_connection.value())));
      return (max_clients != 0 && ++connected == max_clients)
                 ? MinimalSocket::samples::PollableStatus::COMPLETED
                 : MinimalSocket::samples::PollableStatus::ADVANCED;
    }
    return MinimalSocket::samples::PollableStatus::NOT_ADVANCED;
  });

  pollables.loop(std::chrono::seconds{5});

  return EXIT_SUCCESS;
}
