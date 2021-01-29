/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <async/AsyncTcpServer.h>

namespace sck::async {
    class AsyncTcpServer::AcceptanceService : public Service {
    public:
        AcceptanceService(AsyncTcpServer& server) 
            : Service([&server](){
                auto client = server.wrapped->acceptClient();
                std::lock_guard<std::mutex> lk(server.listenerMtx);
                if(nullptr != server.listener) {
                    server.listener->handle(std::move(client));
                }
            }) {
        }
    };

    std::unique_ptr<Service> AsyncTcpServer::make_service() {
        return std::make_unique<AcceptanceService>(*this);
    }
}
