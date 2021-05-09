/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <core/components/ChannelAware.h>
#include "../../Channel.h"

namespace sck {
    ChannelAware::~ChannelAware() {
        this->channel.reset();
    }
}
