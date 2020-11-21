#include <ProcessLauncher.h>

int main() {

   Launcher lnc("launcher");
   lnc.addProcess("Sample03_Client", "500");
   lnc.addProcess("Sample03_Client", "50");
   lnc.addProcess("Sample03_Server");
   lnc();

   return 0;
}