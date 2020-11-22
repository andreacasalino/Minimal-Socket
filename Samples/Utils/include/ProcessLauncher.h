/**
* Author:    Andrea Casalino
* Created:   16.05.2019
*
* report any bug to andrecasa91@gmail.com.
**/

#ifndef _PROCESS_LAUCNHER_
#define _PROCESS_LAUCNHER_

#include <fstream>
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

#endif