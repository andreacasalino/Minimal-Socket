#include <ProcessLauncher.h>

int main() {

   Launcher lnc("launcher");
   lnc.addProcess("Sample04_Server");
   lnc.addProcess("Sample04_Repeater");
   lnc.addProcess("Sample04_Client");
   lnc();

   return 0;
}