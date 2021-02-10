/**
* Author:    Andrea Casalino
* Created:   16.05.2019
*
* report any bug to andrecasa91@gmail.com.
**/

#ifndef SAMPLE_LAUNCHER_H
#define SAMPLE_LAUNCHER_H

#include <string>
#include <list>
#include <utility>

/**    
 * @brief The object is used to print a script, that is able
 * to launch in a precise sequence certain processes
 */
class Launcher {
public:
    /**    
     * @param[in] the name of script to print
     */
   Launcher(const std::string& nameFile) : nameFile(nameFile) {};

    /** 
     * @brief Add 1 process to launch, with possibly some additional command arguments.
     * @param[in] the name of the process to launch
     * @param[in] the list of arguments to pass when launching the process
     */
   template <typename ... Args>
   void addProcess(const std::string& procName, Args ... args) {
      std::list<std::string> parsed;
      this->parseArgs(parsed, args ...);

      this->commands.push_back({0, procName, parsed});
   };

   /**
    * @brief Add 1 process to launch, precedeed by a tunable sleep, with possibly some additional command arguments.
    * @param[in] the name of the process to launch and the sleep in seconds
    * @param[in] the list of arguments to pass when launching the process
    */
   template <typename ... Args>
   void addProcessSleep(const std::pair<std::string, unsigned int>& procNameSleepTime, Args ... args) {
       std::list<std::string> parsed;
       this->parseArgs(parsed, args ...);

       this->commands.push_back({ procNameSleepTime.second, procNameSleepTime.first, parsed });
   };

    /** 
     * @brief Print the script and runs it
     */
   void  operator()() const;

private:

   template <typename ... Args>
   void parseArgs(std::list<std::string>& parsed , const std::string& arg, Args ... args) {
      parsed.push_back(arg);
      parseArgs(parsed, args ...);
   };

   template <typename ... Args>
   void parseArgs(std::list<std::string>& parsed, const std::string& arg) {
      parsed.push_back(arg);
   };

   void parseArgs(std::list<std::string>& parsed) {
      return;
   };

   struct Info {
       unsigned int msSleep;
       std::string processName;
       std::list<std::string> processArguments;
   };

   std::string nameFile;
   std::list<Info> commands;
};

#include <fstream>

void  Launcher::operator()() const {
    std::string name = this->nameFile;
#ifdef _WIN32
    name += ".bat";
#elif  __linux__
    name += ".sh";
#endif

    std::ofstream f(name);
    auto addSleep = [&f](const unsigned int& sleep) {
        if (0 == sleep) return;
#ifdef _WIN32
        f << std::endl << "timeout /t " << sleep;
#else
        f << std::endl << "sleep " << sleep;
#endif
    };

    auto it = this->commands.begin();
    auto itEnd = this->commands.end();
    --itEnd;
    for (it; it != itEnd; ++it) {
        addSleep(it->msSleep);
#ifdef _WIN32
        f << std::endl << "start \"\" \"" << it->processName << "\"";
        for(auto a : it->processArguments) f << " \"" << a << "\"";
#elif  __linux__
        f << std::endl << "gnome-terminal -x sh -c \"./" << it->first;
        for(auto a : it->processArguments) f << " " << a;
        f << "; bash\"";
#endif
    }
    addSleep(it->msSleep);
#ifdef _WIN32
    f << std::endl << "\"" << this->commands.back().processName << "\"";
    for(auto a : this->commands.back().processArguments) f << " \"" << a << "\"";
#elif  __linux__
    f << std::endl << "./" << this->commands.back().first;
    for(auto a : this->commands.back().processArguments) f << " " << a;
#endif

    f.close();
};

#endif