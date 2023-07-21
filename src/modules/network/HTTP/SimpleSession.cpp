#include "SimpleSession.hpp"

namespace network::http {


SimpleSession::SimpleSession(
  tcp::socket&& socket,
  SimpleSession::ResponseLoader &&loader
) :
  stream_{std::move(socket)},
  loader_{std::move(loader)}
{
  //empty
}

   
void SimpleSession::startSession() {
  asio::dispatch(
    stream_.get_executor(),
    std::bind_front(
      &SimpleSession::readStream,
      shared_from_this()
    )
  );
}

  
void SimpleSession::readStream() {
  
  request_ = {};

  stream_.expires_after(std::chrono::seconds(30));
    
  http::async_read(
    stream_,
    buffer_,
    request_,
    std::bind_front(
      &SimpleSession::processRequest,
      shared_from_this()
    )
  );
}


void SimpleSession::processRequest(
  boost::system::error_code errorCode,
  std::size_t bytes_transferred
) {

  boost::ignore_unused(bytes_transferred);

  if(errorCode == http::error::end_of_stream) {
    endSession();
    return;
  }

  if (errorCode) {
    return;
  };

  loader_(
    std::move(request_),
    std::bind_front(
      &SimpleSession::sendResponse,
      shared_from_this()
    )
  );
\
}


void SimpleSession::sendResponse(http::message_generator&& message) {
  
  bool keep_alive = message.keep_alive();

  beast::async_write(
    stream_,
    std::move(message),
    std::bind_front(
      &SimpleSession::responseStatus,
      shared_from_this(),
      keep_alive
    )
  );
}


void SimpleSession::responseStatus(
  bool keep_alive,
  boost::system::error_code errorCode,
  std::size_t bytesTransferred
) {
  boost::ignore_unused(bytesTransferred);
      
  if (errorCode)
    return /*fail(ec, "write")*/;

  if (keep_alive) {
    readStream();
  } else {
    endSession();
  }
}

void SimpleSession::endSession() {
  boost::system::error_code errorCode;
  stream_.socket().shutdown(tcp::socket::shutdown_send, errorCode);
}


}