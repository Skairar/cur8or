#pragma once

#include <string_view>
#include <filesystem>
#include <unordered_map>
#include <functional>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>


namespace network::http {


namespace beast = boost::beast;
namespace http = beast::http;
namespace asio = boost::asio;
using tcp = boost::asio::ip::tcp; 


class SimpleSession : public std::enable_shared_from_this<SimpleSession>
{
public:

  using RequestType = http::request<http::string_body>;

  using ResponseCallback = std::function<void(http::message_generator)>;

  using ResponseLoader = std::function<
    void(RequestType&&, ResponseCallback&&)
  >;

  SimpleSession(
    tcp::socket&& socket,
    ResponseLoader&& loader
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
  ResponseLoader loader_;

};



}