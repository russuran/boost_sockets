#ifndef GET_LOCAL_IP_HPP
#define GET_LOCAL_IP_HPP

#include <string>
#include <boost/asio.hpp>

inline std::string get_local_ip() {
    using boost::asio::ip::tcp;
    boost::asio::io_context io_context;

    tcp::socket socket(io_context);
    socket.open(tcp::v4());
    socket.bind(tcp::endpoint(tcp::v4(), 0));

    return socket.local_endpoint().address().to_string();
}

#endif // GET_LOCAL_IP_HPP
