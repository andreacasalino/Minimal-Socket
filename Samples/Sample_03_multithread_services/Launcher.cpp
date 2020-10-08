#include "../Utils2.h"

int main() {

   Launcher lnc("launcher");
   lnc.addProcessLaunch("Sample03_Client_A.exe");
   lnc.addProcessLaunch("Sample03_Client_B.exe");
   lnc.addProcessLaunch("Sample03_Server.exe");
   lnc();

   return 0;
}