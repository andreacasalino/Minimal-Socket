#include "../Launcher.h"

int main() {

    Launcher lnc("Launcher");
    lnc.addProcess("UdpResponderAsync", "10000", "15000");
    lnc.addProcessSleep(std::make_pair("UdpAsker", 1), "15000", "250", "10000");
    lnc.run();

    return EXIT_SUCCESS;
}