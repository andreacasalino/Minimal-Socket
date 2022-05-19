/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <ScriptGenerator.h>

#include <fstream>
#include <sstream>

namespace MinimalSocket::samples {
namespace {
#ifdef _WIN32
static const std::string SCRIPT_EXTENSION = std::string{".bat"};
#elif defined(__linux__) || defined(__APPLE__)
static const std::string SCRIPT_EXTENSION = std::string{".sh"};
#endif

std::string to_string(const ProcessAndArgs &subject) {
  std::stringstream stream;
#ifdef _WIN32
  stream << '\"' << subject.process_name << '\"';
  for (const auto &[name, val] : subject.arguments) {
    stream << " \"--" << name << "\" \"" << val << "\"";
  }
#elif defined(__linux__) || defined(__APPLE__)
  stream << "./" << subject.process_name;
  for (const auto &[name, val] : subject.arguments) {
    stream << " --" << name << ' ' << val;
  }
#endif
  return stream.str();
}

void add_process(std::ofstream &stream, const ProcessAndArgs &proc_and_args,
                 const bool new_terminal) {
#ifdef _WIN32
  if (new_terminal) {
    stream << "start \"\" ";
  }
  stream << to_string(proc_and_args);
#elif defined(__linux__) || defined(__APPLE__)
  if (new_terminal) {
    stream << "gnome-terminal -x sh -c \"" << to_string(proc_and_args)
           << " ; bash\"";
  } else {
    stream << to_string(proc_and_args);
  }
#endif
  stream << std::endl;
}
} // namespace

void ScriptGenerator::generate(const std::string &file_name) {
  std::ofstream stream(file_name + SCRIPT_EXTENSION);

#if defined(__linux__) || defined(__APPLE__)
  stream << "#!/bin/sh" << std::endl;
#endif

  for (std::size_t k = 0; k < (processes.size() - 1); ++k) {
    add_process(stream, processes[k], true);
  }
  add_process(stream, processes.back(), false);
}
} // namespace MinimalSocket::samples
