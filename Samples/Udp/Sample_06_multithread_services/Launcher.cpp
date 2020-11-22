#include <ProcessLauncher.h>

int main() {

   Launcher lnc("launcher");
   lnc.addProcess("Sample06_Server");
   lnc.addProcess("Sample06_Client", "500" "3500");
   lnc.addProcess("Sample06_Client", "50" "4500");
   lnc();

   return 0;
}