/**
 * Author:    Andrea Casalino
 * Created:   16.05.2019
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <ScriptGenerator.h>

int main() {
  MinimalSocket::samples::ScriptGenerator server_client_launcher;

  server_client_launcher.add(MinimalSocket::samples::ProcessAndArgs{
      "proc_a",
      std::unordered_map<std::string, std::string>{{"name1", "val1"}}});

  server_client_launcher.add(MinimalSocket::samples::ProcessAndArgs{
      "proc_b", std::unordered_map<std::string, std::string>{
                    {"name1", "val1"}, {"name2", "val2"}}});

  server_client_launcher.generate("Temp");

  return EXIT_SUCCESS;
}