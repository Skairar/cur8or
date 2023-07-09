#include "HTTPSession.hpp"

namespace network {


HTTPSession::HTTPSession(
  tcp::socket&& socket,
  ResponseBuilder<http::string_body, http::fields> &&builder
) :
  stream_{std::move(socket)},
  builder_{std::move(builder)}
{
  //empty
}

   
void HTTPSession::startSession() {
  asio::dispatch(
    stream_.get_executor(),
    std::bind_front(
      &HTTPSession::readStream,
      shared_from_this()
    )
  );
}

  
void HTTPSession::readStream() {
  
  request_ = {};

  stream_.expires_after(std::chrono::seconds(30));
    
  http::async_read(
    stream_,
    buffer_,
    request_,
    std::bind_front(
      &HTTPSession::processRequest,
      shared_from_this()
    )
  );
}


void HTTPSession::processRequest(
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

  builder_.setRequest(std::move(request_));
  builder_.processRequest();
    
  sendResponse(builder_.getResponse());

}


void HTTPSession::sendResponse(http::message_generator&& message) {
  
  bool keep_alive = message.keep_alive();

  beast::async_write(
    stream_,
    std::move(message),
    std::bind_front(
      &HTTPSession::responseStatus,
      shared_from_this(),
      keep_alive
    )
  );
}


void HTTPSession::responseStatus(
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

void HTTPSession::endSession() {
  boost::system::error_code errorCode;
  stream_.socket().shutdown(tcp::socket::shutdown_send, errorCode);
}


}