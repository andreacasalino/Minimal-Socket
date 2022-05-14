/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <MinimalSocket/

#include <Names.h>

namespace MinimalSocket::test {
class Asker : public Logger {
public:
  Asker(std::unique_ptr<sck::Messanger> socket);

  void ask();

  void ask(const std::size_t times);

  void askForever(const std::chrono::milliseconds &sampleTime);

private:
  std::unique_ptr<sck::Messanger> socket;
  NamesMap cursor;
  char recvBuffer[1000];
};
} // namespace MinimalSocket::test
