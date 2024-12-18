#ifndef GET_LOCAL_IP_HPP
#define GET_LOCAL_IP_HPP

#include <string>
#include <boost/asio.hpp>

std::string get_local_ip() {
    using boost::asio::ip::udp;
    boost::asio::io_context io_context;

    udp::socket socket(io_context);
    
    socket.open(udp::v4());
    socket.bind(udp::endpoint(udp::v4(), 0));

    return socket.local_endpoint().address().to_string();
}

#endif //GET_LOCAL_IP_HPP
