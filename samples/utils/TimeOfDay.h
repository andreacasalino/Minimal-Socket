/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <chrono>
#include <ostream>

namespace MinimalSocket::samples {
struct TimeOfDay {
  TimeOfDay() : TimeOfDay{time(NULL)} {}
  TimeOfDay(const time_t &t);

  int hours;
  int minutes;
  int seconds;
};

std::ostream &operator<<(std::ostream &the_stream, const TimeOfDay &subject);
} // namespace MinimalSocket::samples
