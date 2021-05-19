/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <Names.h>
#include <sstream>

namespace sck::sample {
    NamesMap::NamesMap() {
        this->cursor = namesSurnames.begin();
    }

    const std::map<std::string, std::string> NamesMap::namesSurnames = {
        {"Luciano", "Pavarotti"},
        {"Gengis", "Khan"},
        {"Giulio", "Cesare"},
        {"Theodor", "Roosvelt"},
        {"Immanuel", "Kant"}
    };

    const std::string NamesMap::unknown = "unknown";

    NamesMap& NamesMap::operator++() {
        ++this->cursor;
        if (this->cursor == namesSurnames.end()) {
            this->cursor = namesSurnames.begin();
        }
        return *this;
    }

    const std::string& NamesMap::getSurname(const std::string& name) {
        auto it = namesSurnames.find(name);
        if (it == namesSurnames.end()) return unknown;
        return it->second;
    }

    bool NamesDecoder::decode(const std::string& buffer, NamesCollection& message) const {
        message.clear();
        if (buffer.empty()) {
            return true;
        }
        std::list<std::size_t> posSeparator;
        for (std::size_t k = 0; k < buffer.size(); ++k) {
            if (';' == buffer[k]) {
                posSeparator.push_back(k);
            }
        }
        if (posSeparator.empty()) {
            message = { buffer };
            return true;
        }
        std::size_t posPrev = 0;
        for (auto it = posSeparator.begin(); it != posSeparator.end(); ++it) {
            message.push_back(std::string(buffer, posPrev, *it - posPrev));
            posPrev = *it + 1;
        }
        if (posPrev < buffer.size()) {
            message.push_back(std::string(buffer, posPrev));
        }
        return true;
    }

    bool NamesEncoder::encode(std::string& buffer, const NamesCollection& message) const {
        std::stringstream stream;
        if (!message.empty()) {
            auto it = message.begin();
            stream << *it;
            ++it;
            for (it; it != message.end(); ++it) {
                stream << ';';
                stream << *it;
            }
        }
        buffer = stream.str();
        return true;
    }
}
