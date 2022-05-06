/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <sstream>
#include <stdexcept>

namespace MinCppSock {
/**
 * @brief A runtime error that can be raised when using any object in sck::
 */
class Error : public std::runtime_error {
public:
  Error(const std::string &what) : std::runtime_error(what){};

  template <typename... Args> Error(Args... args) : Error(merge(args...)) {}

private:
  template <typename... Args> static std::string merge(Args... args) {
    std::stringstream stream;
    merge(stream, args...);
    return stream.str();
  };

  template <typename T, typename... Args>
  static void merge(std::stringstream &stream, const T &current,
                    Args... remaining) {
    stream << current;
    merge(stream, remaining...);
  };

  template <typename T, typename... Args>
  static void merge(std::stringstream &stream, const T &back) {
    stream << back;
  };
};
} // namespace MinCppSock
