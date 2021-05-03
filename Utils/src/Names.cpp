/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <Names.h>

namespace sck::sample {
    Names::Names() {
        this->cursor = namesSurnames.begin();
    }

    const std::map<std::string, std::string> Names::namesSurnames = {
        {"Luciano", "Pavarotti"},
        {"Gengis", "Khan"},
        {"Giulio", "Cesare"},
        {"Theodor", "Roosvelt"},
        {"Immanuel", "Kant"}
    };

    const std::string Names::unknown = "unknown";

    Names& Names::operator++() {
        ++this->cursor;
        if (this->cursor == namesSurnames.end()) {
            this->cursor = namesSurnames.begin();
        }
        return *this;
    }

    const std::string& Names::getSurname(const std::string& name) {
        auto it = namesSurnames.find(name);
        if (it == namesSurnames.end()) return unknown;
        return it->second;
    }
}
