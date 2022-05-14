/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <string>
#include <unordered_map>

namespace MinimalSocket::test {
using Names = std::unordered_multimap<std::string, std::string>;
static const Names NAMES_SURNAMES;

using NamesIterator = Names::const_iterator;

class NamesCircularIterator {
public:
  NamesCircularIterator() : current_(NAMES_SURNAMES.begin()){};

  const NamesIterator &current() { return current_; };

  void next();

private:
  NamesIterator current_;
};
} // namespace MinimalSocket::test
