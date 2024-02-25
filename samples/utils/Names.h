/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <string>
#include <unordered_map>

namespace MinimalSocket::samples {

class NamesCircularIterator {
public:
  using Names = std::unordered_map<std::string, std::string>;
  using NamesIterator = Names::const_iterator;
  static inline Names NAMES_SURNAMES = {{"Luciano", "Pavarotti"},
                                        {"Gengis", "Khan"},
                                        {"Giulio", "Cesare"},
                                        {"Theodor", "Roosvelt"},
                                        {"Immanuel", "Kant"}};

  NamesCircularIterator() = default;

  const NamesIterator &current() { return current_; };

  void next();

private:
  NamesIterator current_ = NAMES_SURNAMES.begin();
};
} // namespace MinimalSocket::samples
