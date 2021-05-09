/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef SAMPLE_LOGGER_H
#define SAMPLE_LOGGER_H

#include <string>
#include <mutex>
#include <sstream>

namespace sck::sample {
    class Logger {
    public:
        Logger(const std::string& logName);

        void log(const std::string& message) const;

        template<typename ... Slices>
        void log(Slices ... slices) const {
            std::stringstream stream;
            pack(stream, slices...);
            this->log(stream.str());
        };

        Logger& operator=(const Logger& ) = delete;

    private:
        Logger(const Logger& o);

        template<typename ... Slices>
        void pack(std::stringstream& stream, const std::string& slice, Slices ... remaining) const {
            stream << slice;
            pack(stream, remaining...);
        };

        template<typename ... Slices>
        void pack(std::stringstream& stream, const std::string& slice) const {
            stream << slice;
        };
        
        const std::string logName;
    };
}

#endif
