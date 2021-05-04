/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <tcpServer/AsyncTcpServer.h>

namespace sck::async {
    AsyncTcpServer::AsyncTcpServer(std::unique_ptr<tcp::TcpServer> server)
        : AsyncSocket(std::move(server)) { 
    };

    void AsyncTcpServer::serviceIteration() {
        auto client = dynamic_cast<tcp::TcpServer*>(this->wrapped.get())->acceptClient();
        //this->Talker<TcpServerListener>::notify(std::move(client));
    }
}
