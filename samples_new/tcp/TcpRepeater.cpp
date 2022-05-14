/**
 * Author:    Andrea Casalino
 * Created:   16.05.2019
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <MinimalSocket/tcp/TcpClient.h>
#include <MinimalSocket/tcp/TcpServer.h>

#include <Args.h>

#include <iostream>
using namespace std;

class Repeater {
public:
  Repeater(MinimalSocket::tcp::TcpClient &&connection_to_preceding,
           MinimalSocket::tcp::TcpConnection &&connection_to_following)
      : preceding(std::move(connection_to_preceding)),
        following(std::move(connection_to_following)) {}

  void repeat() {
    auto request = following.receive(500);
    preceding.send(request);
    auto response = preceding.receive(500);
    following.send(response);
  }

private:
  MinimalSocket::tcp::TcpClient preceding;
  MinimalSocket::tcp::TcpConnection following;
};

int main(const int argc, const char **argv) {
  cout << "-----------------------  Repeater  -----------------------" << endl;
  PARSE_ARGS

  const auto preceding_host = options->getValue("host", "127.0.0.1");
  const auto preceding_port = static_cast<MinimalSocket::Port>(
      std::atoi(options->getValue("prec_port").c_str()));
  MinimalSocket::Address preceding_address(preceding_host, preceding_port);

  const auto port_to_reserve = static_cast<MinimalSocket::Port>(
      std::atoi(options->getValue("port").c_str()));

  // ask connection to preceding
  MinimalSocket::tcp::TcpClient connection_to_preceding(preceding_address);
  if (connection_to_preceding.open()) {
    cout << "Unable to connect to preceding" << endl;
    return EXIT_FAILURE;
  }
  // wait connection request from follower
  MinimalSocket::tcp::TcpServer acceptor(port_to_reserve,
                                         preceding_address.getFamily());
  if (acceptor.open()) {
    cout << "Unable to reserve port" << std::to_string(port_to_reserve) << endl;
    return EXIT_FAILURE;
  }
  auto connection_to_following = acceptor.acceptNewClient();

  Repeater repeater(std::move(connection_to_preceding),
                    std::move(connection_to_following));
  while (true) {
    repeater.repeat();
  }

  return EXIT_SUCCESS;
}