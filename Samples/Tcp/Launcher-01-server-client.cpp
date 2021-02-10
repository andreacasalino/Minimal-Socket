#include "../ProcessLauncher.h"

int main() {

   Launcher lnc("Launcher01");
   lnc.addProcess("TcpServer","20000");
   lnc.addProcess("TcpClient","20000");
   lnc();

   return EXIT_SUCCESS;
}