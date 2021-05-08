#include "../Launcher.h"

int main() {

   Launcher lnc("Launcher");
   lnc.addProcess("TcpServerAsync", "45000");
   lnc.addProcessSleep(std::make_pair("TcpClient", 1), "45000", "250");
   lnc.run();

   return EXIT_SUCCESS;
}