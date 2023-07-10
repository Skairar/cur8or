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

public:

  ResponseBuilder(std::filesystem::path workingDirectory);

  void setRequest(RequestType &&request);
  http::message_generator getResponse();

private:

  std::filesystem::path workingDirectory_;
  RequestType request_;
  
  http::message_generator errorResponse(http::status status, std::string_view message);

};


}


#include "ResponseBuilder.tpp"