#include "../Launcher.h"

int main() {

   Launcher lnc("Launcher");
   lnc.addProcess("TcpServer", "25000", "2");
   lnc.addProcessSleep(std::make_pair("TcpClient", 1), "25000", "100");
   lnc.addProcess("TcpClient","25000", "500");
   lnc.run();

   return EXIT_SUCCESS;
}