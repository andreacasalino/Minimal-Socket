#include <ProcessLauncher.h>

int main() {

   Launcher lnc("launcher");
   lnc.addProcess("Sample01_Server");
   lnc.addProcess("Sample01_Client");
   lnc();

   return 0;
}