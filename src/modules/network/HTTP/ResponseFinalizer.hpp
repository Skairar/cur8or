#pragma once

#include <memory>
#include <functional>
#include <cstdint>

#include <boost/beast/http/message.hpp>
#include <boost/beast/http/message_generator.hpp>
#include <boost/beast/http/string_body.hpp>

#include "ResponseTemplate.hpp"

namespace network::http {

/*
  Helper class for MessageProcessor and ResponseTemplate that makes sure
  both request and callback passed to a query are kept alive as long as
  they're needed.
  It is supposed to take full ownership of both request and callback.
  The three callback functions it employs pass request further, so
  only one of them should be invoked for any existing instance of this object
  and only once, it's intended to end its lifecycle afterwards.
*/
class ResponseFinalizer
  : public std::enable_shared_from_this<ResponseFinalizer>
{
public:

  //all of its typedefs are dependent on those in ResponseTemplate
  using RequestType = ResponseTemplate::RequestType;

  using ResponseType = ResponseTemplate::ResponseType;

  using StatusType = ResponseTemplate::StatusType;

  using PathType = ResponseTemplate::PathType;

  using ResponseCallback = std::function<
    void(ResponseType&&)
  >;

  ResponseFinalizer(
    RequestType&& request,
    ResponseCallback&& callback
  );

  RequestType& getRequest() noexcept;

  void errorResponseCallback(std::string_view message);

  void fileResponseCallback(const PathType& path);

  void dataResponseCallback(std::string&& data);

private:

  RequestType request_;

  ResponseCallback callback_;

};



}