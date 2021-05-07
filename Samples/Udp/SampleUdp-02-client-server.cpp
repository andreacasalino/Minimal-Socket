#include "../ProcessLauncher.h"

int main() {

   Launcher lnc("Launcher02");
   lnc.addProcess("UdpServerResponder", "35000");
   lnc.addProcessSleep(std::make_pair("UdpAsker", 1), "35000", "250", "30000", "1");
   lnc();

   return EXIT_SUCCESS;
}