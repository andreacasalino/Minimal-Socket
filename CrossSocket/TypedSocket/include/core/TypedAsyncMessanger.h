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
#include <core/TypedSender.h>
#include <core/TypedMessangerListener.h>
#include <core/components/Decoder.h>

namespace sck::typed {
    template<typename SendT, typename Encoder_, typename RecvT, typename Decoder_>
    class TypedAsynchMessanger
        : public StateAware
        , public Openable
        , public Closable
        , protected async::MessangerListener
        , public TypedSender<SendT, Encoder_>
        , public async::Talker<TypedMessangerListener<RecvT>>
        , protected Decoder_ {
        static_assert(std::is_base_of<Decoder<RecvT>, Decoder_>::value, "Not valid Decoder_ type");
    public:
        TypedAsynchMessanger(std::unique_ptr<Connection> messanger, const std::size_t& bufferCapacity)
            : asyncMessanger(std::move(messanger), bufferCapacity) {
            this->sender = &this->asyncMessanger;
            this->asyncMessanger.async::MessageTalker::resetListener(this);
        };

        inline bool isOpen() const override { return this->asyncMessanger.isOpen(); };

        inline void close() override { this->asyncMessanger.close(); };

        inline void open(const std::chrono::milliseconds& timeout) override { this->asyncMessanger.open(timeout); };

        inline void resetErrorListener(async::ErrorListener* listener) { this->asyncMessanger.async::ErrorTalker::resetListener(listener); };

    protected:
        void handle(const std::pair<const char*, std::size_t>& message) final {
            RecvT typed;
            if (this->Decoder_::decode(std::string(message.first, message.second), typed)) {
                this->notify(typed);
            }
        }

    private:
        async::AsyncMessanger asyncMessanger;
    };
}

#endif
#endif
