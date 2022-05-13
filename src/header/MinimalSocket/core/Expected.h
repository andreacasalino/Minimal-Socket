/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <MinimalSocket/Error.h>

#include <functional>
#include <memory>

namespace MinimalSocket {
template <typename T> class Expected {
public:
  using T_Ptr = std::unique_ptr<T>;
  using Error_Ptr = std::unique_ptr<Error>;

  Expected(const Expected &) = delete;
  Expected &operator=(const Expected &) = delete;

  Expected(T_Ptr value) : value(std::move(value)){};
  Expected(Error_Ptr exception) : exception(std::move(exception)){};

  const T *value() const { return value.get(); };
  T *value() { return value.get(); };

  const Error *error() const { return exception.get(); };

  void visit(
      const std::function<void(const T &)> &value_case = [](const T &) {},
      const std::function<void(const Error &)> &exception_case =
          [](const Error &) {}) const {
    if (nullptr == value) {
      exception_case(*exception.get());
    } else {
      value_case(*value.get());
    }
  }

private:
  T_Ptr value;
  Error_Ptr exception;
};
} // namespace MinimalSocket
