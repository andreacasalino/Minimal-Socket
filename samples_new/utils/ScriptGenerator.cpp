/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <ScriptGenerator.h>

#include <fstream>

namespace MinimalSocket::samples {
namespace {
#ifdef _WIN32
static const std::string SCRIPT_EXTENSION = std::string{".bat"};
#elif __linux__
static const std::string SCRIPT_EXTENSION = std::string{".sh"};
#endif
} // namespace

void ScriptGenerator::generate(const std::string &file_name) {
  std::string complete_file_name = file_name + SCRIPT_EXTENSION;
  std::ofstream stream(complete_file_name);
  for (const auto &process : processes) {
#ifdef _WIN32
    stream << "start \"\" \"" << process.process_name << "\"";
    for (const auto &[name, val] : process.arguments) {
      stream << " \"--" << name << "\" \"" << val << "\"";
    }
#elif __linux__
    stream << "gnome-terminal -x sh -c \"./" << process.process_name;
    for (const auto &[name, val] : process.arguments) {
      stream << " --" << name << ' ' << val;
    }
    stream << " ; bash\"";
#endif
    stream << std::endl;
  }

  // "start \"\" \""
}
} // namespace MinimalSocket::samples
