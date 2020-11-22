#include <ProcessLauncher.h>

int main() {

   Launcher lnc("launcher");
   lnc.addProcess("Sample05_Server");
   lnc.addProcess("Sample05_Client");
   lnc();

   return 0;
}