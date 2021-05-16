/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifdef ASYNCH_ENABLED
#ifndef _CROSS_SOCKET_TYPEDASYNCMESSANGER_H_
#define _CROSS_SOCKET_TYPEDASYNCMESSANGER_H_

#include <messanger/AsyncMessanger.h>
#include <core/TypedReceiver.h>
#include <core/TypedSender.h>
#include <core/TypedMessangerListener.h>

namespace sck::typed {
    template<typename SendT, typename Encoder_, typename RecvT, typename Decoder_>
    class TypedAsynchMessanger
        : protected AsyncMessanger
        , public Talker<TypedMessangerListener<RecvT>>
        , public TypedSender<SendT, Encoder_>
        , public Decoder_ {
    public:
        TypedAsynchMessanger(std::unique_ptr<Connection> messanger, const std::size_t& bufferCapacity);
    };
}

#endif
#endif
