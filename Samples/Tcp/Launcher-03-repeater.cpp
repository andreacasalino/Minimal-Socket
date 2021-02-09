#include "../ProcessLauncher.h"

int main() {

   Launcher lnc("Launcher03");
   lnc.addProcess("TcpServer","3000");
   lnc.addProcess("TcpRepeater");
   lnc.addProcess("TcpClient","4000");
   lnc();

   return EXIT_SUCCESS;
}