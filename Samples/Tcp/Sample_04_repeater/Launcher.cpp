#include <ProcessLauncher.h>

int main() {

   Launcher lnc("launcher");
   lnc.addProcess("Sample04_Client");
   lnc.addProcess("Sample04_Repeater");
   lnc.addProcess("Sample04_Server");
   lnc();

   return 0;
}