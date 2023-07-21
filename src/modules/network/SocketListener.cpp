#include "SocketListener.hpp"

#include <boost/asio/strand.hpp>

namespace network {


void SocketListener::acceptUpcoming() {
  connectionAcceptor_.async_accept(
    asio::make_strand(executionContext_),
    std::bind_front(
      &SocketListener::runHandlerOnSocket,
      shared_from_this()
    )
  );
}


SocketListener::SocketListener(
  asio::io_context& executionContext,
  tcp::endpoint listenedSocket,
  std::function<void(tcp::socket&&)> &&connectionHandler
) :
  executionContext_{executionContext},
  connectionAcceptor_{asio::make_strand(executionContext)},
  connectionHandler_{std::move(connectionHandler)}
{
  boost::system::error_code errorCode;
        
  connectionAcceptor_.open(listenedSocket.protocol(), errorCode);
  if(errorCode)
  {
    //fail(ec, "open");
    return;
  }

  connectionAcceptor_.set_option(asio::socket_base::reuse_address(true), errorCode);
  if(errorCode)
  {
    //fail(ec, "set_option");
    return;
  }

  connectionAcceptor_.bind(listenedSocket, errorCode);
  if(errorCode)
  {
    //fail(ec, "bind");
    return;
  }

  // Start listening for connections
  connectionAcceptor_.listen(
    asio::socket_base::max_listen_connections, errorCode);
  if(errorCode)
  {
    //fail(ec, "listen");
    return;
  }
}


void SocketListener::runHandlerOnSocket(
  boost::system::error_code errorCode,
  tcp::socket socket
) {
  if(errorCode)
  {
    //fail(ec, "accept");
    return; // To avoid infinite loop
  }
  else
  {
    /*
    std::make_shared<session>(
      std::move(socket),
      interfaceRoot_)->run();
    }*/
    connectionHandler_(std::move(socket));

    acceptUpcoming();
  }
}


}