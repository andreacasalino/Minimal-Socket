#include "../Launcher.h"

int main() {

   Launcher lnc("Launcher");
   lnc.addProcess("UdpServer", "35000");
   lnc.addProcessSleep(std::make_pair("UdpAsker", 1), "35000", "250", "30000", "1");
   lnc.run();

   return EXIT_SUCCESS;
}