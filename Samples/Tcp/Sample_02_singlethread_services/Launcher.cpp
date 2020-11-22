#include <ProcessLauncher.h>

int main() {

   Launcher lnc("launcher");
   lnc.addProcess("Sample02_Server");
   lnc.addProcess("Sample02_Client");
   lnc.addProcess("Sample02_Client");
   lnc();

   return 0;
}