#include "../Utils2.h"

int main() {

   Launcher lnc("launcher");
   lnc.addProcessLaunch("Sample02_Client_A");
   lnc.addProcessLaunch("Sample02_Client_B");
   lnc.addProcessLaunch("Sample02_Server");
   lnc();

   return 0;
}