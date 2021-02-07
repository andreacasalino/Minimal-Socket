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

      this->commands.emplace_back(std::make_pair(procName, parsed));
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

   std::string nameFile;
   std::list<std::pair<std::string, std::list<std::string>>> commands;
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

//#ifdef _WIN32
//    system(name.c_str());
//#elif  __linux__
//    system(std::string("sh ./" + name).c_str());
//#endif
};

#endif