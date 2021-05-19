#include <gtest/gtest.h>
#include <TcpCommon.h>
#include <omp.h>
using namespace sck;
using namespace sck::tcp;

#include <NamesMap.h>
#include <core/TypedMessanger.h>
#include <NamesCoding.h>
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

TEST(TcpTyped, ClientAsker_ServerResponder) {
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
                sample::Names response;
                for (auto it = request.begin(); it != request.end(); ++it) {
                    response.push_back(sample::NamesMap::getSurname(*it));
                }
                EXPECT_TRUE(messanger.send(response));
            }
#pragma omp barrier
        }
        else {
            // client
            std::unique_ptr<TcpClient> client = std::make_unique<TcpClient>(*sck::Ip::createLocalHost(port));
            sample::openTcpClient(*client);
            TypedMessanger<sample::Names, sample::NamesEncoder, sample::Names, sample::NamesDecoder> messanger(std::move(client), 2500);
            // exchange typed messages
            for (std::size_t k = 0; k < cycles; ++k) {
                EXPECT_TRUE(messanger.send(getNames()));
                sample::Names response;
                EXPECT_TRUE(messanger.receive(response, std::chrono::milliseconds(0)));
                EXPECT_EQ(response, getSurnames());
            }
#pragma omp barrier
        }
    }
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
