#include "../Launcher.h"

int main() {

   Launcher lnc("Launcher");
   lnc.addProcess("TcpServer","3000");
   lnc.addProcessSleep(std::make_pair("TcpRepeater", 1));
   lnc.addProcessSleep(std::make_pair("TcpClient", 1), "4000");
   lnc.run();

   return EXIT_SUCCESS;
}