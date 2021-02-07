/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef SAMPLE_NAMES_H
#define SAMPLE_NAMES_H

#include <map>
#include <string>

class Names {
public:
    Names() {
		this->cursor = namesSurnames.begin();
	}

    inline const std::string& getCursorName() const { return this->cursor->first; };
    inline const std::string& getCursorSurname() const { return this->cursor->second; };

    Names& operator++() {
		++this->cursor;
		if (this->cursor == namesSurnames.end()) {
			this->cursor = namesSurnames.begin();
		}
		return *this;
	}

private:
    static const std::map<std::string, std::string> namesSurnames;

    std::map<std::string, std::string>::const_iterator cursor;
};

const std::map<std::string, std::string> Names::namesSurnames = {
    {"Luciano", "Pavarotti"},
    {"Gengis", "Khan"},
    {"Giulio", "Cesare"},
    {"Theodor", "Roosvelt"},
    {"Immanuel", "Kant"}
};

#endif
