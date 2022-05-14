/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <iostream>
#include <optional>
#include <string>
#include <unordered_map>

namespace MinimalSocket::samples {
using ArgsMap = std::unordered_map<std::string, std::string>;

// Group the passed args into an ordered table
class Args {
public:
  Args(Args &&) = default;

  static std::optional<Args> parse(const int argc, const char **argv);

  // default value is returned in case the argument name is not found among the
  // parsed ones
  std::string getValue(const std::string &argument_name,
                       const std::string &default_value) const;

  // throw if this option does not exists
  std::string getValue(const std::string &argument_name) const;

private:
  Args(const int argc, const char **argv);

  ArgsMap arguments_map;
};

#define PARSE_ARGS                                                             \
  auto options = MinimalSocket::samples::Args::parse(argc, argv);              \
  if (std::nullopt == options) {                                               \
    std::cout << "Invalid arguments" << std::endl;                             \
    return EXIT_FAILURE;                                                       \
  }
} // namespace MinimalSocket::samples
