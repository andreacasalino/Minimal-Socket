#include <TimeOfDay.h>

#include <chrono>

namespace MinimalSocket::samples {
TimeOfDay::TimeOfDay(const time_t &t) {
  struct tm *tmp = gmtime(&t);
  hours = tmp->tm_hour;
  minutes = tmp->tm_min;
  seconds = tmp->tm_sec;
}

std::ostream &operator<<(std::ostream &the_stream, const TimeOfDay &subject) {
  auto print = [&the_stream](int val) {
    if (val < 10) {
      the_stream << '0';
    }
    the_stream << val;
  };

  print(subject.hours);
  the_stream << ':';
  print(subject.minutes);
  the_stream << ':';
  print(subject.seconds);
  return the_stream << ": ";
}
} // namespace MinimalSocket::samples
