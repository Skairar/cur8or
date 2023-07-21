#pragma once
#include <memory>
#include <functional>

#include <boost/system/error_code.hpp>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>


namespace network {


namespace asio = boost::asio;
using tcp = boost::asio::ip::tcp;


class SocketListener : public std::enable_shared_from_this<SocketListener>
{
public:

  SocketListener(
    asio::io_context& executionContext,
    tcp::endpoint listenedSocket,
    std::function<void(tcp::socket&&)> &&connectionHandler
  );

  void acceptUpcoming();

private:

    asio::io_context &executionContext_;
    tcp::acceptor connectionAcceptor_;
    std::function<void(tcp::socket&&)> connectionHandler_;

    void runHandlerOnSocket(boost::system::error_code errorCode, tcp::socket socket);

};


}