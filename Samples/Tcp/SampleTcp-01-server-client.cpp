#include "../Launcher.h"

int main() {

   Launcher lnc("Launcher");
   lnc.addProcess("TcpServer", "20000");
   lnc.addProcessSleep(std::make_pair("TcpClient", 1), "20000");
   lnc.run();

   return EXIT_SUCCESS;
}