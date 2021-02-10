#include "../ProcessLauncher.h"

int main() {

   Launcher lnc("Launcher02");
   lnc.addProcess("UpdServer", "35000");
   lnc.addProcess("UdpClientAsker", "35000", "250", "30000", "1");
   lnc();

   return EXIT_SUCCESS;
}