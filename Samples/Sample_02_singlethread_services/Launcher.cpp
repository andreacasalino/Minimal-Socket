#include "../Utils/include/ProcessLauncher.h"

int main() {

   Launcher lnc("launcher");
   lnc.addProcess("Sample02_Client");
   lnc.addProcess("Sample02_Client");
   lnc.addProcess("Sample02_Server");
   lnc();

   return 0;
}