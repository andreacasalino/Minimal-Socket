#include "../Utils2.h"

int main() {

   Launcher lnc("launcher");
   lnc.addProcessLaunch("Sample01_Client.exe");
   lnc.addProcessLaunch("Sample01_Server.exe");
   lnc();

   return 0;
}