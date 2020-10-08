#include "../Utils2.h"

int main() {

   Launcher lnc("launcher");
   lnc.addProcessLaunch("Sample04_Client.exe");
   lnc.addProcessLaunch("Sample04_Repeater.exe");
   lnc.addProcessLaunch("Sample04_Server.exe");
   lnc();

   return 0;
}