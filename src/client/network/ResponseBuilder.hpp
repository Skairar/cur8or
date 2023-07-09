#pragma once

#include <string_view>
#include <filesystem>
#include <functional>

#include <boost/beast/http/message_generator.hpp>



namespace network {


namespace http = boost::beast::http;


std::string_view fileMimeType(const std::filesystem::path &filePath) noexcept;

template <class Body>
class ResponseBuilder
{

  using RequestType = http::request<Body>;
  using ResponseType = http::message_generator;

public:

  ResponseBuilder(std::filesystem::path workingDirectory);

  void setRequest(RequestType &&request);
  void processRequest();
  ResponseType&& getResponse();

private:

  std::filesystem::path workingDirectory_;
  RequestType request_;
  ResponseType response_;
  
  
  void errorResponse(http::status status, std::string_view message);

};


}