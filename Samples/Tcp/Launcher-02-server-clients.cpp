#include "../ProcessLauncher.h"

int main() {

   Launcher lnc("Launcher02");
   lnc.addProcess("TcpServer","25000", "2");
   lnc.addProcess("TcpClient","25000", "100");
   lnc.addProcess("TcpClient","25000", "500");
   lnc();

   return EXIT_SUCCESS;
}