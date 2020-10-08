#include "../Utils2.h"

int main() {

   Launcher lnc("launcher");
   lnc.addProcessLaunch("Sample01_Client");
   lnc.addProcessLaunch("Sample01_Server");
   lnc();

   return 0;
}