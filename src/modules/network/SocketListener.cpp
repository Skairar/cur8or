#include "SocketListener.hpp"

#include <boost/asio/strand.hpp>
#include <boost/system/error_code.hpp>


namespace network{



SocketListener::SocketListener(
  boost::asio::io_context& executionContext,
  boost::asio::ip::tcp::endpoint listenedSocket,
  std::function<void(boost::asio::ip::tcp::socket&&)>&& connectionHandler
) noexcept
 : executionContext_{executionContext},
   connectionAcceptor_{boost::asio::make_strand(executionContext)},
   connectionHandler_{std::move(connectionHandler)}
{
  
  boost::system::error_code errorCode;
        
  connectionAcceptor_.open(listenedSocket.protocol(), errorCode);
  if (errorCode) {
    return;
  }

  connectionAcceptor_.set_option(
    boost::asio::socket_base::reuse_address(true),
    errorCode
  );
  if (errorCode) {
    return;
  }

  connectionAcceptor_.bind(listenedSocket, errorCode);
  if (errorCode) {
    return;
  }

  // Start listening for connections
  connectionAcceptor_.listen(
    boost::asio::socket_base::max_listen_connections,
    errorCode
  );
  if (errorCode) {
    return;
  }

}



void SocketListener::acceptUpcoming() noexcept{

  connectionAcceptor_.async_accept(
    boost::asio::make_strand(executionContext_),
    std::bind_front(
      &SocketListener::runHandlerOnSocket,
      shared_from_this()
    )
  );

}



void SocketListener::runHandlerOnSocket(
  boost::system::error_code errorCode,
  boost::asio::ip::tcp::socket socket
) noexcept{

  if (errorCode) {
    return; // To avoid infinite loop
  } else {
    connectionHandler_(std::move(socket));

    acceptUpcoming();
  }

}



} //namespace network