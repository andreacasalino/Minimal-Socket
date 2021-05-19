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
#ifdef TYPED_ENABLED
#include <core/components/Encoder.h>
#include <core/components/Decoder.h>
#include <list>
#endif

namespace sck::sample {
    class NamesMap {
    public:
        NamesMap();

        inline const std::string& getCursorName() const { return this->cursor->first; };
        inline const std::string& getCursorSurname() const { return this->cursor->second; };

        static const std::string& getSurname(const std::string& name);

        NamesMap& operator++();

        inline static std::size_t size() { return namesSurnames.size(); };

    private:
        static const std::map<std::string, std::string> namesSurnames;
        static const std::string unknown;

        std::map<std::string, std::string>::const_iterator cursor;
    };

#ifdef TYPED_ENABLED
    typedef std::list<std::string> NamesCollection;

    class NamesDecoder : public typed::Decoder<NamesCollection> {
    protected:
        bool decode(const std::string& buffer, NamesCollection& message) const final;
    };

    class NamesEncoder : public typed::Encoder<NamesCollection> {
    protected:
        bool encode(std::string& buffer, const NamesCollection& message) const final;
    };
#endif
}

#endif
