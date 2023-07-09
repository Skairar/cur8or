#pragma once

#include "ResponseBuilder.hpp"

#include <boost/system/error_code.hpp>

#include <boost/beast/http/message.hpp>
#include <boost/beast/http/file_body.hpp>
#include <boost/beast/http/empty_body.hpp>
#include <boost/beast/version.hpp>


namespace network {


namespace beast = boost::beast;


template <class Body>
ResponseBuilder<Body>::ResponseBuilder(
  std::filesystem::path workingDirectory
) : workingDirectory_{workingDirectory},
    request_{}
{
  //empty
}


template <class Body>
void ResponseBuilder<Body>::setRequest(
  ResponseBuilder<Body>::RequestType &&request
) {
  request_ = std::move(request);
}


template <class Body>
void ResponseBuilder<Body>::processRequest() {
    
  if (request_.method() != http::verb::get &&
      request_.method() != http::verb::head &&
      request_.method() != http::verb::post) {
    errorResponse(
      http::status::bad_request,
      "Unknown HTTP-method"
    );
    return;
  }

  if (request_.target().empty() ||
      request_.target()[0] != '/' ||
      request_.target().find("..") != std::string_view::npos) {
    errorResponse(
      http::status::bad_request,
      "Illegal request-target"
    );
    return;
  }

  std::filesystem::path path = workingDirectory_;
  path.concat(std::string_view{request_.target()});
  if (std::filesystem::is_directory(path)) {
    path /= "index.html";
  }

  boost::system::error_code errorCode;
  http::file_body::value_type body;
  body.open(path.string().c_str(), beast::file_mode::scan, errorCode);

  if (errorCode == boost::system::errc::no_such_file_or_directory) {
    errorResponse(
      http::status::not_found,
      request_.target()
    );
    return;
  }

  if (errorCode) {
    errorResponse(
      http::status::internal_server_error,
      errorCode.message()
    );
    return;
  }

  auto const size = body.size();

  if (request_.method() == http::verb::head)
  {
    http::response<http::empty_body> response{http::status::ok, request_.version()};
    response.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    response.set(http::field::content_type, fileMimeType(path));
    response.content_length(size);
    response.keep_alive(request_.keep_alive());
    response_ = response;
    return;
  }

  if (request_.method() == http::verb::post) {
    http::response<http::file_body> response{
        std::piecewise_construct,
        std::make_tuple(std::move(body)),
        std::make_tuple(http::status::ok, request_.version())};
    response.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    response.set(http::field::content_type, fileMimeType(path));
    response.content_length(size);
    response.keep_alive(request_.keep_alive());
    response_ = response;
    return;
  }
}


template <class Body>
ResponseBuilder<Body>::ResponseType&&
  ResponseBuilder<Body>::getResponse() {
  return response_;
}


template <class Body>
void
  ResponseBuilder<Body>::errorResponse(
    http::status status,
    std::string_view message
  ) {
  http::response<http::string_body> response{status, request_.version()};
  response.set(http::field::server, BOOST_BEAST_VERSION_STRING);
  response.set(http::field::content_type, "text/html");
  response.keep_alive(request.keep_alive());
  response.body() = message;
  response.prepare_payload();
  response_ = response;
}


}