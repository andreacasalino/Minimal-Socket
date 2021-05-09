/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <NamesMap.h>

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
}
