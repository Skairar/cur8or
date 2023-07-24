#include "ResponseTemplate.hpp"

#include <boost/system/error_code.hpp>

#include <boost/beast/http/file_body.hpp>
#include <boost/beast/http/empty_body.hpp>
#include <boost/beast/version.hpp>

#include "helpers/FileMimeType.hpp"


namespace network::http {


ResponseTemplate::ResponseTemplate(ResponseTemplate::RequestType&& request)
 : request_{std::move(request)}
{
  //empty
}


ResponseTemplate::ResponseType ResponseTemplate::errorResponse(
  ResponseTemplate::StatusType status,
  std::string_view message
) {
  using namespace boost::beast::http;
  response<string_body> response{status, request_.version()};
  response.set(field::server, BOOST_BEAST_VERSION_STRING);
  response.set(field::content_type, "text/html");
  response.keep_alive(request_.keep_alive());
  response.body() = message;
  response.prepare_payload();
  return response;
}


ResponseTemplate::ResponseType ResponseTemplate::fileResponse(
  ResponseTemplate::PathType path
) {
  
  using namespace boost::beast;
  using namespace boost::beast::http;
  
  boost::system::error_code errorCode;
  file_body::value_type body;
  body.open(path.string().c_str(), file_mode::scan, errorCode);
 
  if (errorCode == boost::system::errc::no_such_file_or_directory){
    return errorResponse(
      StatusType::not_found, 
      request_.target()
    );
  }
  if (errorCode) {
    return errorResponse(
      status::internal_server_error,
      errorCode.message()
    );
  }
  
  auto const bodySize = body.size();

  if (request_.method() == verb::head) {
    response<empty_body> response{status::ok, request_.version()};
    response.set(field::server, BOOST_BEAST_VERSION_STRING);
    response.set(field::content_type, helpers::fileMimeType(path));
    response.content_length(bodySize);
    response.keep_alive(request_.keep_alive());
    return response;
  } else {
    response<file_body> response{
        std::piecewise_construct,
        std::make_tuple(std::move(body)),
        std::make_tuple(status::ok, request_.version())};
    response.set(field::server, BOOST_BEAST_VERSION_STRING);
    response.set(field::content_type, helpers::fileMimeType(path));
    response.content_length(bodySize);
    response.keep_alive(request_.keep_alive());
    return response;
  }

}


ResponseTemplate::ResponseType ResponseTemplate::dataResponse(std::string&& data) {
  using namespace boost::beast::http;
  response<string_body> response{status::ok, request_.version()};
  response.set(field::server, BOOST_BEAST_VERSION_STRING);
  response.set(field::content_type, "application/json");
  response.keep_alive(request_.keep_alive());
  response.body() = std::move(data);
  response.prepare_payload();
  return response;
}



}