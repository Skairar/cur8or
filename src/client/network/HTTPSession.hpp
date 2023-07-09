#pragma once;

#include <string_view>
#include <filesystem>
#include <unordered_map>
#include <functional>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>

#include "ResponseBuilder.hpp"


namespace network {


namespace beast = boost::beast;
namespace http = beast::http;
namespace asio = boost::asio;
using tcp = boost::asio::ip::tcp; 


class HTTPSession : public std::enable_shared_from_this<HTTPSession>
{
public:

  HTTPSession(
    tcp::socket&& socket,
    ResponseBuilder<http::string_body, http::fields> &&builder
  );

   
  void startSession();
  void readStream();
  void processRequest(
    boost::system::error_code errorCode,
    std::size_t bytes_transferred
  );
  void sendResponse(http::message_generator&& message);
  void responseStatus(
    bool keep_alive,
    boost::system::error_code errorCode,
    std::size_t bytesTransferred
  );
  void endSession();

private:

  beast::tcp_stream stream_;
  beast::flat_buffer buffer_;
  http::request<http::string_body> request_;
  ResponseBuilder<http::string_body> builder_;

};



}