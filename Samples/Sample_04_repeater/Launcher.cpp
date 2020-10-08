#include "../Utils2.h"

int main() {

   Launcher lnc("launcher");
   lnc.addProcessLaunch("Sample04_Client");
   lnc.addProcessLaunch("Sample04_Repeater");
   lnc.addProcessLaunch("Sample04_Server");
   lnc();

   return 0;
}