#include "../ProcessLauncher.h"

int main() {

   Launcher lnc("Launcher01");
   lnc.addProcess("UdpResponder", "10000", "15000");
   lnc.addProcessSleep(std::make_pair("UdpAsker", 1), "15000", "250", "10000");
   lnc();

   return EXIT_SUCCESS;
}