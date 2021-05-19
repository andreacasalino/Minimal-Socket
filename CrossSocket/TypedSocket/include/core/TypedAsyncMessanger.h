///**
// * Author:    Andrea Casalino
// * Created:   01.28.2020
// *
// * report any bug to andrecasa91@gmail.com.
// **/
//
//#ifdef ASYNCH_ENABLED
//#ifndef _CROSS_SOCKET_TYPEDASYNCMESSANGER_H_
//#define _CROSS_SOCKET_TYPEDASYNCMESSANGER_H_
//
//#include <messanger/AsyncMessanger.h>
//#include <core/components/Decoder.h>
//#include <core/TypedSender.h>
//#include <core/TypedMessangerListener.h>
//
//namespace sck::typed {
//    class TypedAsynchMessanger
//        : protected async::AsyncMessanger
//        , protected async::MessangerListener
//        , public async::Talker<TypedMessangerListener<RecvT>>
//        , public TypedSender<SendT, Encoder_>
//        , protected Decoder_ {
//    public:
//        TypedAsynchMessanger(std::unique_ptr<Connection> messanger, const std::size_t& bufferCapacity)
//            : AsyncMessanger(std::move(messanger), bufferCapacity) {
//            this->async::MessageTalker::resetListener(this);
//        };
//
//        inline bool isOpen() const { return this->async::AsyncMessanger::isOpen(); };
//
//        inline void close() { this->async::AsyncMessanger::close(); };
//
//        inline void open(const std::chrono::milliseconds& timeout) { this->async::AsyncMessanger::open(timeout); };
//
//        inline void resetErrorListener(async::ErrorListener* listener) { this->async::AsyncMessanger::resetListener(listener); };
//
//    protected:
//        void handle(const std::pair<const char*, std::size_t>& message) final {
//            // bool decode(const std::string& buffer, T& message)
//            RecvT typed;
//            this->Decoder_::decode(std::string(message.first, message.second), typed);
//            this->notify(typed);
//        }
//    };
//}
//
//#endif
//#endif
