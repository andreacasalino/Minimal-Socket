#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#if _WIN64 || _WIN32
#include <windows.h>
#endif

class Popen {
public:
  Popen(const std::string &line) {
    fp =
#if _WIN64 || _WIN32
        _popen(line.c_str(), "r")
#elif __linux__
        popen(line.c_str(), "r")
#endif
        ;
    if (fp == NULL) {
      throw std::runtime_error{"Unable to start the child process"};
    }
  }

  struct CompletedProcess {
    int returnCode;
    std::string out;
  };
  CompletedProcess communicate() {
    if (!done) {
      while (poll()) {
      }
      done = true;
    }
    return CompletedProcess{returnCode_, out_.str()};
  }

protected:
  bool poll() {
    std::string temp;
    temp.resize(250);
    if (fgets(temp.data(), static_cast<int>(temp.size()), fp) == nullptr) {
#if _WIN64 || _WIN32
      feof(fp);
#endif

      returnCode_ =
#if _WIN64 || _WIN32
          _pclose(fp)
#elif __linux__
          pclose(fp)
#endif
          ;

      fp = nullptr;
      return false;
    }
    std::size_t last_pos = temp.find('\n');
    if (last_pos != std::string::npos) {
      temp.resize(last_pos + 1);
    }
    out_ << temp;
    return true;
  }

private:
  bool done = false;
  FILE *fp = nullptr;
  std::string buffer_;
  int returnCode_ = -1;
  std::stringstream out_;
};

#include <string_view>
#include <vector>

const std::string cmd{CMD};
std::vector<std::string_view> getCommands() {
  std::vector<std::string_view> res;
  std::size_t pos = 0;
  while (pos < cmd.size()) {
    auto next = cmd.find('|', pos);
    if (next == std::string::npos) {
      res.emplace_back(cmd.data() + pos);
      break;
    }
    res.emplace_back(cmd.data() + pos, next - pos);
    pos = next + 1;
  }
  return res;
}

int main() {
  for (const auto &proc : getCommands()) {
    Popen process(std::string{proc.data(), proc.size()});
    auto &&[retCode, out] = process.communicate();
    std::cout << out;
    if (retCode != 0) {
      throw std::runtime_error{"Child process didn't complete well"};
    }
  }

  return EXIT_SUCCESS;
}
