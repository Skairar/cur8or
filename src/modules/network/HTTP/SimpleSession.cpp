#include "SimpleSession.hpp"


namespace network::http {



SimpleSession::SimpleSession(
  boost::asio::ip::tcp::socket&& socket,
  SimpleSession::ResponseLoader &&loader
) noexcept
 : stream_{std::move(socket)},
   loader_{std::move(loader)}
{
  //empty
}


   
void SimpleSession::startSession() noexcept{

  boost::asio::dispatch(
    stream_.get_executor(),
    std::bind_front(
      &SimpleSession::readStream,
      shared_from_this()
    )
  );

}


  
void SimpleSession::readStream() noexcept{
  
  request_ = {};

  stream_.expires_after(std::chrono::seconds(30));
    
  boost::beast::http::async_read(
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
) noexcept{

  boost::ignore_unused(bytes_transferred);

  if(errorCode == boost::beast::http::error::end_of_stream) {
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

}



void SimpleSession::sendResponse(
  boost::beast::http::message_generator&& message
) noexcept{
  
  bool keep_alive = message.keep_alive();

  boost::beast::async_write(
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
) noexcept{

  boost::ignore_unused(bytesTransferred);
      
  if (errorCode) {
    return;
  }
  if (keep_alive) {
    readStream();
  } else {
    endSession();
  }

}



void SimpleSession::endSession() noexcept{

  boost::system::error_code errorCode;
  stream_.socket().shutdown(
    boost::asio::ip::tcp::socket::shutdown_send,
    errorCode
  );

}



}//namespace network::http