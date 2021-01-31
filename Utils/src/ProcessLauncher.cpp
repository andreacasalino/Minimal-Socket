#include "../include/ProcessLauncher.h"
#include <stdexcept>

Launcher::Launcher(const std::string& nameFile) : nameFile(nameFile) {
#ifdef _WIN32
#elif  __linux__
#elif 
    throw std::runtime_error("Unrecognized operative system");
#endif
}; 

void  Launcher::operator()() const {
    std::string name = this->nameFile;
#ifdef _WIN32
    name += ".bat";
#elif  __linux__
    name += ".sh";
#endif

    std::ofstream f(name);
    auto it = this->commands.begin();
    auto itEnd = this->commands.end();
    --itEnd;
    for (it; it != itEnd; ++it) {
#ifdef _WIN32
        f << std::endl << "start \"\" \"" << it->first << "\"";
        for(auto a : it->second) f << " \"" << a << "\"";
#elif  __linux__
        f << std::endl << "gnome-terminal -x sh -c \"./" << it->first;
        for(auto a : it->second) f << " " << a;
        f << "; bash\"";
#endif
    }
#ifdef _WIN32
    f << std::endl << "\"" << this->commands.back().first << "\""; 
    for(auto a : this->commands.back().second) f << " \"" << a << "\"";
#elif  __linux__
    f << std::endl << "./" << this->commands.back().first;
    for(auto a : this->commands.back().second) f << " " << a;
#endif

    f.close();

#ifdef _WIN32
    system(name.c_str());
#elif  __linux__
    system(std::string("sh ./" + name).c_str());
#endif
};