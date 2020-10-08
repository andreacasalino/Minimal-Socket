/**
 * Author:    Andrea Casalino
 * Created:   16.05.2019
*
* report any bug to andrecasa91@gmail.com.
 **/

#ifndef _UTILS2_
#define _UTILS2_

#include <fstream>
#include <string>
#include <list>

class Launcher {
public:
   Launcher(const std::string& nameFile) : nameFile(nameFile) {};

   template <typename ... Args>
   void addProcessLaunch(const std::string& procName, Args ... args) {
      std::list<std::string> parsed;
      this->parseArgs(parsed, args ...);

      this->lines.push_back(procName + "\"");
      for (auto it = parsed.begin(); it != parsed.end(); ++it) {
         this->lines.back() += " \"";
         this->lines.back() += *it;
         this->lines.back() += "\"";
      }

   };

   void  operator()() const {
      std::string name = this->nameFile;
#ifdef _WIN32
      name += ".bat";
#elif  __linux__
      name += ".sh";
#endif

      std::ofstream f(name);
      auto it = this->lines.begin();
      auto itEnd = this->lines.end();
      --itEnd;
      for (it; it != itEnd; ++it) {
#ifdef _WIN32
         f << std::endl << "start \"\" ";
#elif  __linux__
         f << std::endl << "gnome-terminal -x sh -c ";
#endif
         f << *it;
      }
      f << std::endl << *it;

      f.close();

#ifdef _WIN32
      system(name.c_str());
#elif  __linux__
      system(std::string("sh ./" + name).c_str());
#endif
   };

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
   std::list<std::string> lines;
};

#endif