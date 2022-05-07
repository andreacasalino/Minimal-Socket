// /**
//  * Author:    Andrea Casalino
//  * Created:   01.28.2020
//  *
//  * report any bug to andrecasa91@gmail.com.
//  **/

// #include <UdpCommon.h>

// namespace sck::sample {
//     std::pair<UdpConnectionPtr, UdpConnectionPtr>
//     makeOpenedUdpConnections(const std::uint16_t portA, const std::uint16_t
//     portB) {
//         UdpConnectionPtr connA =
//         std::make_unique<udp::UdpConnection>(*Ip::createLocalHost(portB) ,
//         portA); UdpConnectionPtr connB =
//         std::make_unique<udp::UdpConnection>(*Ip::createLocalHost(portA) ,
//         portB);

//         open(*connA);
//         open(*connB);

//         return std::make_pair(std::move(connA), std::move(connB));
//     };
// }
