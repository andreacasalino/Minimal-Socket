/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <Logger.h>
#include <Error.h>
#include <iostream>

namespace sck::sample {
    static std::set<std::string> logNamesReserved;

    Logger::Logger(const std::string& logName)
        : logName(logName) {
        if (logNamesReserved.find(logName) != logNamesReserved.end()) {
            throw Error("Names already reserved");
        }
        logNamesReserved.emplace(logName);
    }

    static std::mutex coutMtx;

    void Logger::log(const std::string& message) const {
        std::lock_guard<std::mutex> coutLk(coutMtx);
        std::cout << this->logName << " : " << message << std::endl;
    }
}
