/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace MinimalSocket::samples {
using ProcessArgs = std::unordered_map<std::string, std::string>;

struct ProcessAndArgs {
  std::string process_name;
  ProcessArgs arguments;
};

class ScriptGenerator {
public:
  ScriptGenerator() = default;

  void add(const ProcessAndArgs &info) { processes.push_back(info); };

  void generate(const std::string &file_name);

private:
  std::vector<ProcessAndArgs> processes;
};
} // namespace MinimalSocket::samples
