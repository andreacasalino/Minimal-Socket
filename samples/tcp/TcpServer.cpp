/**
 * Author:    Andrea Casalino
 * Created:   16.05.2019
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <MinimalSocket/tcp/TcpServer.h>

#include <Args.h>
#include <Respond.h>

#include <future>

using namespace std;

struct ActiveConnection {
  MinimalSocket::tcp::TcpConnection channel;
  std::future<void> task;
};
void accept_new_client(MinimalSocket::tcp::TcpServer &server,
                       std::list<ActiveConnection> &active_connections) {
  MinimalSocket::tcp::TcpConnection accepted_connection =
      server.acceptNewClient();
  cout << "New client accepted" << endl;

  auto *channel = &active_connections
                       .emplace_back(ActiveConnection{
                           std::move(accepted_connection), std::future<void>{}})
                       .channel;
  active_connections.back().task = std::async([channel = channel]() {
    MinimalSocket::samples::respond_forever(*channel);
  });
}

int main(const int argc, const char **argv) {
  cout << "-----------------------  Server  -----------------------" << endl;
  PARSE_ARGS

  const auto server_port = static_cast<MinimalSocket::Port>(
      std::atoi(options->getValue("port").c_str()));
  const auto max_clients = std::atoi(options->getValue("clients", "0").c_str());
  const auto family =
      MinimalSocket::samples::to_family(options->getValue("family", "v4"));

  MinimalSocket::tcp::TcpServer server(server_port, family);

  if (!server.open()) {
    cout << "Failed to bind and listen to specified port" << endl;
    return EXIT_FAILURE;
  }
  cout << "Listening for new clients on port " << server_port << endl;

  std::list<ActiveConnection> active_connections;
  auto accept_clients_task = std::async([&]() {
    if (0 == max_clients) {
      while (true) {
        accept_new_client(server, active_connections);
      }

    } else {
      for (int c = 0; c < max_clients; ++c) {
        accept_new_client(server, active_connections);
      }
    }
  });

  accept_clients_task.get();
  for (auto &active_connection : active_connections) {
    try {
      active_connection.task.get();
    } catch (...) {
    }
  }

  return EXIT_SUCCESS;
}
