#include "../Utils2.h"

int main() {

   Launcher lnc("launcher");
   lnc.addProcessLaunch("Sample03_Client_A");
   lnc.addProcessLaunch("Sample03_Client_B");
   lnc.addProcessLaunch("Sample03_Server");
   lnc();

   return 0;
}