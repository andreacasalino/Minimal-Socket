/**
 * Author:    Andrea Casalino
 * Created:   16.05.2019
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <ScriptGenerator.h>

#include <iostream>
using namespace std;

int main() {
  {
    // 1 server 1 client
    const std::string sample_name = "tcp01_server_client";
    MinimalSocket::samples::ScriptGenerator generator;

    const std::string port = "35998";

    generator.add("TcpServer", {{"port", port}, {"clients", "1"}});

    generator.add("TcpClient", {{"port", port}});

    cout << "generating " << sample_name << endl;
    generator.generate(sample_name);
  }

  {
    // 1 server 2 clients
    const std::string sample_name = "tcp02_server_2_clients";
    MinimalSocket::samples::ScriptGenerator generator;

    const std::string port = "35998";

    generator.add("TcpServer", {{"port", port}});

    generator.add("TcpClient", {{"port", port}});

    generator.add("TcpClient", {{"port", port}, {"rate", "800"}});

    cout << "generating " << sample_name << endl;
    generator.generate(sample_name);
  }

  {
    // repeaters
    const std::size_t repeaters = 2;
    const std::string sample_name =
        "tcp03_chain_size_" + std::to_string(repeaters + 2);
    MinimalSocket::samples::ScriptGenerator generator;

    std::size_t port = 35998;

    generator.add("TcpServer",
                  {{"port", std::to_string(port)}, {"clients", "1"}});

    for (std::size_t r = 0; r < repeaters; ++r) {
      auto new_port = port + 10;
      generator.add("TcpRepeater", {{"port", std::to_string(new_port)},
                                    {"next_port", std::to_string(port)}});
      port = new_port;
    }

    generator.add("TcpClient", {{"port", std::to_string(port)}});

    cout << "generating " << sample_name << endl;
    generator.generate(sample_name);
  }

  return EXIT_SUCCESS;
}