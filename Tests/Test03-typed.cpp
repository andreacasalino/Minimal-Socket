#include <gtest/gtest.h>
#include <TcpCommon.h>
#include <omp.h>
using namespace sck;
using namespace sck::tcp;

#include <Names.h>
#include <core/TypedMessanger.h>
#include <core/TypedAsyncMessanger.h>
#include <Logger.h>
using namespace sck::typed;

constexpr std::size_t BUFFER_CAPACITY = 2500;

sample::NamesCollection getAllNames() {
    sample::NamesCollection names;
    sample::NamesMap map;
    for (std::size_t k = 0; k < sample::NamesMap::size(); ++k) {
        names.push_back(map.getCursorName());
        ++map;
    }
    return names;
};

sample::NamesCollection getAllSurnames() {
    sample::NamesCollection surnames;
    sample::NamesMap map;
    for (std::size_t k = 0; k < sample::NamesMap::size(); ++k) {
        surnames.push_back(map.getCursorSurname());
        ++map;
    }
    return surnames;
};

sample::NamesCollection getResponse(const sample::NamesCollection& request) {
    sample::NamesCollection response;
    for (auto it = request.begin(); it != request.end(); ++it) {
        response.push_back(sample::NamesMap::getSurname(*it));
    }
    return response;
};

void doRequests(TypedMessanger<sample::NamesCollection, sample::NamesEncoder, sample::NamesCollection, sample::NamesDecoder>& messanger, const std::size_t cycles) {
    for (std::size_t k = 0; k < cycles; ++k) {
        EXPECT_TRUE(messanger.send(getAllNames()));
        sample::NamesCollection response;
        EXPECT_TRUE(messanger.receive(response, std::chrono::milliseconds(0)));
        EXPECT_EQ(response, getAllSurnames());
    }
}

TEST(Typed, Sync) {
    const std::uint16_t port = sample::PortFactory::makePort();
    const std::size_t cycles = 5;

#pragma omp parallel num_threads(2)
    {
        if (0 == omp_get_thread_num()) {
            // server
            auto acceptedClient = sample::accept(port);
            TypedMessanger<sample::NamesCollection, sample::NamesEncoder, sample::NamesCollection, sample::NamesDecoder> messanger(std::move(acceptedClient), BUFFER_CAPACITY);
            // exchange typed messages
            for (std::size_t k = 0; k < cycles; ++k) {
                sample::NamesCollection request;
                EXPECT_TRUE(messanger.receive(request, std::chrono::milliseconds(0)));
                EXPECT_TRUE(messanger.send(getResponse(request)));
            }
#pragma omp barrier
        }
        else {
            // client
            std::unique_ptr<TcpClient> client = std::make_unique<TcpClient>(*sck::Ip::createLocalHost(port));
            sample::openTcpClient(*client);
            TypedMessanger<sample::NamesCollection, sample::NamesEncoder, sample::NamesCollection, sample::NamesDecoder> messanger(std::move(client), BUFFER_CAPACITY);
            doRequests(messanger, cycles);
#pragma omp barrier
        }
    }
}


std::string toString(const sample::NamesCollection& names) {
    std::stringstream stream;
    if (!names.empty()) {
        auto it = names.begin();
        stream << *it;
        ++it;
        for (it; it != names.end(); ++it) {
            stream << ';' << *it;
        }
    }
    return stream.str();
};

class TypedAsyncResponder
    : public TypedAsynchMessanger<sample::NamesCollection, sample::NamesEncoder, sample::NamesCollection, sample::NamesDecoder>
    , public TypedMessangerListener<sample::NamesCollection>
    , protected sck::async::ErrorListener
    , public sample::Logger {
public:
    TypedAsyncResponder(std::unique_ptr<Connection> messanger) 
        : TypedAsynchMessanger<sample::NamesCollection, sample::NamesEncoder, sample::NamesCollection, sample::NamesDecoder>(std::move(messanger), BUFFER_CAPACITY)
        , Logger("TypedAsyncResponder") {
        this->resetErrorListener(this);
        this->resetListener(this);
    };

private:
    void handle(const sample::NamesCollection& message) final {
        this->log("request: ", toString(message));
        sample::NamesCollection response = getResponse(message);
        this->log("response: ", toString(response));
        this->send(response);
    };

    void handle(const sck::Error& error) final {};

    void handle(const std::exception& error) final {};
};

TEST(Typed, Async) {
    const std::uint16_t port = sample::PortFactory::makePort();
    const std::size_t cycles = 5;

#pragma omp parallel num_threads(2)
    {
        if (0 == omp_get_thread_num()) {
            // server
            auto acceptedClient = sample::accept(port);
            TypedAsyncResponder asynchResponder(std::move(acceptedClient));
            open(asynchResponder);
#pragma omp barrier
            close(asynchResponder);
        }
        else {
            // client
            std::unique_ptr<TcpClient> client = std::make_unique<TcpClient>(*sck::Ip::createLocalHost(port));
            sample::openTcpClient(*client);
            TypedMessanger<sample::NamesCollection, sample::NamesEncoder, sample::NamesCollection, sample::NamesDecoder> messanger(std::move(client), BUFFER_CAPACITY);
            doRequests(messanger, cycles);
#pragma omp barrier
        }
    }
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
