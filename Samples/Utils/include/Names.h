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
    Names();

    inline const std::string& getCursorName() const { return this->cursor->first; };
    inline const std::string& getCursorSurname() const { return this->cursor->second; };

    static const std::string& getSurname(const std::string& name);

    Names& operator++();

private:
    static const std::map<std::string, std::string> namesSurnames;
    static const std::string unknown;

    std::map<std::string, std::string>::const_iterator cursor;
};

#endif
