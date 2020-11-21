#include "../Utils/include/ProcessLauncher.h"

int main() {

   Launcher lnc("launcher");
   lnc.addProcess("Sample01_Client");
   lnc.addProcess("Sample01_Server");
   lnc();

   return 0;
}