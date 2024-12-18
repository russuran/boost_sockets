#ifndef HASING_HPP
#define HASING_HPP_HPP

#include <boost/asio.hpp>
#include <unordered_map>
#include <functional>
#include <string>

using boost::asio::ip::udp;

struct EndpointHash {
    std::size_t operator()(const udp::endpoint& endpoint) const {
        return std::hash<std::string>()(endpoint.address().to_string() + ":" + std::to_string(endpoint.port()));
    }
};

struct EndpointEqual {
    bool operator()(const udp::endpoint& lhs, const udp::endpoint& rhs) const {
        return lhs.address() == rhs.address() && lhs.port() == rhs.port();
    }
};

#endif
