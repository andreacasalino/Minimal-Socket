/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef SAMPLE_NAMESCODER_H
#define SAMPLE_NAMESCODER_H

#include <core/components/Encoder.h>
#include <core/components/Decoder.h>
#include <list>

namespace sck::sample {
    typedef std::list<std::string> Names;

    class NamesDecoder : public typed::Decoder<Names> {
    protected:
        bool decode(const std::string& buffer, Names& message) const final;
    };

    class NamesEncoder : public typed::Encoder<Names> {
    protected:
        bool encode(std::string& buffer, const Names& message) const final;
    };
}

#endif
