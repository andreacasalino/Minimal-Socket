#include <gtest/gtest.h>
#include <TcpCommon.h>
#include <omp.h>
using namespace sck;
using namespace sck::tcp;

#include <NamesMap.h>
#include <core/TypedMessanger.h>
#include <core/TypedAsyncMessanger.h>
#include <NamesCoding.h>
#include <Logger.h>
using namespace sck::typed;

sample::Names getNames() {
    sample::Names names;
    sample::NamesMap map;
    for (std::size_t k = 0; k < sample::NamesMap::size(); ++k) {
        names.push_back(map.getCursorName());
        ++map;
    }
    return names;
};

sample::Names getSurnames() {
    sample::Names surnames;
    sample::NamesMap map;
    for (std::size_t k = 0; k < sample::NamesMap::size(); ++k) {
        surnames.push_back(map.getCursorSurname());
        ++map;
    }
    return surnames;
};

sample::Names getResponse(const sample::Names& request) {
    sample::Names response;
    for (auto it = request.begin(); it != request.end(); ++it) {
        response.push_back(sample::NamesMap::getSurname(*it));
    }
    return response;
};

void doRequests(TypedMessanger<sample::Names, sample::NamesEncoder, sample::Names, sample::NamesDecoder>& messanger, const std::size_t cycles) {
    for (std::size_t k = 0; k < cycles; ++k) {
        EXPECT_TRUE(messanger.send(getNames()));
        sample::Names response;
        EXPECT_TRUE(messanger.receive(response, std::chrono::milliseconds(0)));
        EXPECT_EQ(response, getSurnames());
    }
}

TEST(SyncTyped, ClientAsker_ServerResponder) {
    const std::uint16_t port = sample::PortFactory::makePort();
    const std::size_t cycles = 5;

#pragma omp parallel num_threads(2)
    {
        if (0 == omp_get_thread_num()) {
            // server
            auto acceptedClient = sample::accept(port);
            TypedMessanger<sample::Names, sample::NamesEncoder, sample::Names, sample::NamesDecoder> messanger(std::move(acceptedClient), 2500);
            // exchange typed messages
            for (std::size_t k = 0; k < cycles; ++k) {
                sample::Names request;
                EXPECT_TRUE(messanger.receive(request, std::chrono::milliseconds(0)));
                EXPECT_TRUE(messanger.send(getResponse(request)));
            }
#pragma omp barrier
        }
        else {
            // client
            std::unique_ptr<TcpClient> client = std::make_unique<TcpClient>(*sck::Ip::createLocalHost(port));
            sample::openTcpClient(*client);
            TypedMessanger<sample::Names, sample::NamesEncoder, sample::Names, sample::NamesDecoder> messanger(std::move(client), 2500);
            doRequests(messanger, cycles);
#pragma omp barrier
        }
    }
}


class TypedAsyncResponder
    : public TypedAsynchMessanger<sample::Names, sample::NamesEncoder, sample::Names, sample::NamesDecoder>
    , public TypedMessangerListener<sample::Names>
    , protected sck::async::ErrorListener
    , public sample::Logger {
public:
    TypedAsyncResponder(std::unique_ptr<Connection> messanger, const std::size_t& bufferCapacity) 
        : TypedAsynchMessanger<sample::Names, sample::NamesEncoder, sample::Names, sample::NamesDecoder>(std::move(messanger), bufferCapacity)
        , Logger("TypedAsyncResponder") {
        this->resetErrorListener(this);
        this->resetListener(this);
    };

private:
    void handle(const sample::Names& message) final {
        sample::Names response = getResponse(message);
        this->send(response);
        //std::string recStr(message.first, message.second);
        //const std::string surname = NamesMap::getSurname(recStr);
        //this->log("request: ", recStr, " response: ", surname);
        //this->messPtr->send({ surname.data(), surname.size() });
    };

    void handle(const sck::Error& error) final {};

    void handle(const std::exception& error) final {};
};

TEST(AsyncTyped, ClientAsker_ServerResponder) {
    const std::uint16_t port = sample::PortFactory::makePort();
    const std::size_t cycles = 5;

#pragma omp parallel num_threads(2)
    {
        if (0 == omp_get_thread_num()) {
            // server
            auto acceptedClient = sample::accept(port);
            TypedAsynchMessanger<sample::Names, sample::NamesEncoder, sample::Names, sample::NamesDecoder> asynchResponder(std::move(acceptedClient), 2500);
            open(asynchResponder);
#pragma omp barrier
            asynchResponder.close();
        }
        else {
            // client
            std::unique_ptr<TcpClient> client = std::make_unique<TcpClient>(*sck::Ip::createLocalHost(port));
            sample::openTcpClient(*client);
            TypedMessanger<sample::Names, sample::NamesEncoder, sample::Names, sample::NamesDecoder> messanger(std::move(client), 2500);
            doRequests(messanger, cycles);
#pragma omp barrier
        }
    }
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
