#include "../ProcessLauncher.h"

int main() {

   Launcher lnc("Launcher04");
   lnc.addProcess("TcpServer","3000");
   lnc.addProcessSleep(std::make_pair("TcpRepeater", 1));
   lnc.addProcessSleep(std::make_pair("TcpClient", 1), "4000");
   lnc();

   return EXIT_SUCCESS;
}