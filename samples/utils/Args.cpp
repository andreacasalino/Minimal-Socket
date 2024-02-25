/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include "Args.h"

#include <sstream>
#include <stdexcept>

namespace MinimalSocket::samples {
std::optional<Args> Args::parse(const int argc, const char **argv) {
  std::optional<Args> result;
  try {
    Args parsed(argc, argv);
    result.emplace(std::move(parsed));
  } catch (...) {
  }
  return result;
}

Args::Args(const int argc, const char **argv) {
  if (((argc - 1) % 2) != 0) {
    throw std::runtime_error{""};
  }
  for (int k = 1; k < argc; k += 2) {
    std::string name = argv[k];
    if (name.size() < 3) {
      throw std::runtime_error{""};
    }
    if ((name[0] != '-') || (name[1] != '-')) {
      throw std::runtime_error{""};
    }

    name = std::string{name, 2};
    std::string value = argv[k + 1];
    if (name.empty()) {
      throw std::runtime_error{""};
    }

    arguments_map[name] = value;
  }

  for (const auto &[name, value] : arguments_map) {
    std::cout << "--" << name << ": " << value << std::endl;
  }
  std::cout << std::endl;
}

MinimalSocket::AddressFamily to_family(const std::string &family_as_string) {
  if (family_as_string == "v4") {
    return MinimalSocket::AddressFamily::IP_V4;
  }
  if (family_as_string == "v6") {
    return MinimalSocket::AddressFamily::IP_V6;
  }
  throw std::runtime_error{"Invalid family address: it can be only v4 or v6"};
}
} // namespace MinimalSocket::samples
