#include "../Launcher.h"

int main() {

   Launcher lnc("Launcher");
   lnc.addProcess("TcpServerAsync", "45000", "2");
   lnc.addProcessSleep(std::make_pair("TcpClient", 1), "45000", "100");
   lnc.addProcess("TcpClient", "45000", "500");
   lnc.run();

   return EXIT_SUCCESS;
}