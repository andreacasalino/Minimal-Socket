#include "../Utils2.h"

int main() {

   Launcher lnc("launcher");
   lnc.addProcessLaunch("Sample02_Client_A.exe");
   lnc.addProcessLaunch("Sample02_Client_B.exe");
   lnc.addProcessLaunch("Sample02_Server.exe");
   lnc();

   return 0;
}