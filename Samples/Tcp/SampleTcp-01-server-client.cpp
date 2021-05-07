#include "../ProcessLauncher.h"

int main() {

   Launcher lnc("Launcher01");
   lnc.addProcess("TcpServer", "20000");
   lnc.addProcessSleep(std::make_pair("TcpClient", 1), "20000");
   lnc();

   return EXIT_SUCCESS;
}