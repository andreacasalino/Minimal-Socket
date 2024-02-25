/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <MinimalSocket/core/Address.h>

#include <chrono>
#include <iostream>
#include <optional>
#include <string>
#include <unordered_map>

namespace MinimalSocket::samples {
template <typename T> struct Convert {};

template <> struct Convert<std::string> {
  static std::string convert(const std::string &val) { return val; }
};

template <> struct Convert<bool> {
  static bool convert(const std::string &val) {
    if (val == "1" || val == "yes" || val == "true") {
      return true;
    }
    if (val == "0" || val == "no" || val == "false") {
      return false;
    }
    throw std::runtime_error{"Unrecognized boolean value"};
  }
};

template <> struct Convert<int> {
  static int convert(const std::string &val) { return std::atoi(val.c_str()); }
};

template <> struct Convert<std::chrono::milliseconds> {
  static std::chrono::milliseconds convert(const std::string &val) {
    return std::chrono::milliseconds{std::atoi(val.c_str())};
  }
};

template <> struct Convert<MinimalSocket::Port> {
  static MinimalSocket::Port convert(const std::string &val) {
    return static_cast<MinimalSocket::Port>(std::atoi(val.c_str()));
  }
};

MinimalSocket::AddressFamily to_family(const std::string &family_as_string);

template <> struct Convert<MinimalSocket::AddressFamily> {
  static MinimalSocket::AddressFamily convert(const std::string &val) {
    return to_family(val);
  }
};

// Group the passed args into an ordered table
class Args {
public:
  static std::optional<Args> parse(const int argc, const char **argv);

  template <typename T>
  T getValue(const std::string &argument_name,
             const std::optional<T> &default_value = std::nullopt) const {
    auto it = arguments_map.find(argument_name);
    if (it == arguments_map.end()) {
      if (default_value == std::nullopt) {
        std::string msg = "Unable to find '" + argument_name + "'";
        throw std::runtime_error{msg};
      }
      return default_value.value();
    }
    return Convert<T>::convert(it->second);
  }

private:
  Args(const int argc, const char **argv);

  std::unordered_map<std::string, std::string> arguments_map;
};

#define PARSE_ARGS                                                             \
  auto options = MinimalSocket::samples::Args::parse(argc, argv);              \
  if (std::nullopt == options) {                                               \
    std::cerr << "Invalid arguments" << std::endl;                             \
    return EXIT_FAILURE;                                                       \
  }
} // namespace MinimalSocket::samples
