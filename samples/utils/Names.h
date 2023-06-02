/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <sstream>
#include <string>
#include <unordered_map>

namespace MinimalSocket::samples {
using Names = std::unordered_map<std::string, std::string>;

using NamesIterator = Names::const_iterator;

class NamesCircularIterator {
public:
  static const Names NAMES_SURNAMES;

  NamesCircularIterator() : current_(NAMES_SURNAMES.begin()){};

  const NamesIterator &current() { return current_; };

  void next();

  static std::size_t size() {
    return NamesCircularIterator::NAMES_SURNAMES.size();
  }

private:
  NamesIterator current_;
};
} // namespace MinimalSocket::samples
