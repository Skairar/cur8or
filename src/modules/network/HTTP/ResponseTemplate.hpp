#pragma once

#include <string>
#include <string_view>
#include <filesystem>
#include <cstdint>

#include <boost/beast/http/message.hpp>
#include <boost/beast/http/message_generator.hpp>
#include <boost/beast/http/string_body.hpp>


namespace network::http {


class ResponseTemplate
{
public:

  using RequestType = boost::beast::http::request<
    boost::beast::http::string_body
  >;

  using ResponseType = boost::beast::http::message_generator;

  using StatusType = boost::beast::http::status;

  using PathType = std::filesystem::path;

  ResponseTemplate(RequestType&& request);

  ResponseType errorResponse(
    StatusType status,
    std::string_view message
  );

  ResponseType fileResponse(
    PathType path
  );

  ResponseType dataResponse(
    std::string&& data
  );

private:

  RequestType request_;

};



}