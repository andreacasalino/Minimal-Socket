/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <Logger.h>
#include <Error.h>
#include <iostream>
#include <map>

namespace sck::sample {
    static std::map<std::string, std::size_t> namesUsed;

    std::string makeName(const std::string& logName) {
        auto it = namesUsed.find(logName);
        if (it == namesUsed.end()) {
            it = namesUsed.emplace(logName, 1).first;
        }
        else {
            ++it->second;
        }
        return logName + std::to_string(it->second);
    }
    Logger::Logger(const std::string& logName)
        : logName(makeName(logName)) {
    }

    static std::mutex coutMtx;

    void Logger::log(const std::string& message) const {
        std::lock_guard<std::mutex> coutLk(coutMtx);
        std::cout << this->logName << " : " << message << std::endl;
    }
}
