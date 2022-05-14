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
    // 1 responder 1 asker
    const std::string sample_name = "udp_responder_asker";
    MinimalSocket::samples::ScriptGenerator generator;

    const std::size_t port_asker = 36995;
    const std::size_t port_responder = port_asker + 10;

    generator.add(MinimalSocket::samples::ProcessAndArgs{
        "UdpResponder", MinimalSocket::samples::ProcessArgs{
                            {"port_this", std::to_string(port_responder)}}});

    generator.add(MinimalSocket::samples::ProcessAndArgs{
        "UdpAsker", MinimalSocket::samples::ProcessArgs{
                        {"port", std::to_string(port_responder)},
                        {"port_this", std::to_string(port_asker)}}});

    cout << "generating " << sample_name << endl;
    generator.generate(sample_name);
  }

  {
    // 1 connecting responder 1 asker
    const std::string sample_name = "udp_connecting_responder_asker";
    MinimalSocket::samples::ScriptGenerator generator;

    const std::size_t port_asker = 36995;
    const std::size_t port_responder = port_asker + 10;

    generator.add(MinimalSocket::samples::ProcessAndArgs{
        "UdpResponder", MinimalSocket::samples::ProcessArgs{
                            {"port_this", std::to_string(port_responder)},
                            {"connect", "yes"}}});

    generator.add(MinimalSocket::samples::ProcessAndArgs{
        "UdpAsker", MinimalSocket::samples::ProcessArgs{
                        {"port", std::to_string(port_responder)},
                        {"port_this", std::to_string(port_asker)}}});

    cout << "generating " << sample_name << endl;
    generator.generate(sample_name);
  }

  {
    // 1 responder 2 askers
    const std::string sample_name = "udp_responder_2_askers";
    MinimalSocket::samples::ScriptGenerator generator;

    const std::size_t port_responder = 36995;
    const std::size_t port_asker_1 = port_responder + 10;
    const std::size_t port_asker_2 = port_responder + 20;

    generator.add(MinimalSocket::samples::ProcessAndArgs{
        "UdpResponder", MinimalSocket::samples::ProcessArgs{
                            {"port_this", std::to_string(port_responder)}}});

    generator.add(MinimalSocket::samples::ProcessAndArgs{
        "UdpAsker", MinimalSocket::samples::ProcessArgs{
                        {"port", std::to_string(port_responder)},
                        {"port_this", std::to_string(port_asker_1)}}});

    generator.add(MinimalSocket::samples::ProcessAndArgs{
        "UdpAsker", MinimalSocket::samples::ProcessArgs{
                        {"port", std::to_string(port_responder)},
                        {"port_this", std::to_string(port_asker_2)},
                        {"rate", "300"}}});

    cout << "generating " << sample_name << endl;
    generator.generate(sample_name);
  }

  return EXIT_SUCCESS;
}