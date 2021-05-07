#include "../ProcessLauncher.h"

int main() {

   Launcher lnc("Launcher03");
   lnc.addProcess("TcpServerAsync", "45000");
   lnc.addProcessSleep(std::make_pair("TcpClient", 1), "45000", "250");
   lnc();

   return EXIT_SUCCESS;
}