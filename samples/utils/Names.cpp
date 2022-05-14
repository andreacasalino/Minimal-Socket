/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <Names.h>

namespace MinimalSocket::samples {
const Names NamesCircularIterator::NAMES_SURNAMES =
    Names{{"Luciano", "Pavarotti"},
          {"Gengis", "Khan"},
          {"Giulio", "Cesare"},
          {"Theodor", "Roosvelt"},
          {"Immanuel", "Kant"}};

void NamesCircularIterator::next() {
  ++current_;
  if (current_ == NAMES_SURNAMES.end()) {
    current_ = NAMES_SURNAMES.begin();
  }
}
} // namespace MinimalSocket::samples
