/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <Names.h>

namespace MinimalSocket::test {
Names NAMES_SURNAMES = {{"Luciano", "Pavarotti"},
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
} // namespace MinimalSocket::test
