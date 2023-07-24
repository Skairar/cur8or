#pragma once

#include <functional>
#include <memory>
#include <cstdint>

#include <boost/beast/http/message.hpp>
#include <boost/beast/http/message_generator.hpp>
#include <boost/beast/http/string_body.hpp>



namespace network::http {


template <class PathResolver>
class MessageProcessor :
  public std::enable_shared_from_this<MessageProcessor<PathResolver>>
{
public:

  using RequestType = boost::beast::http::request<
    boost::beast::http::string_body
  >;

  using ResponseCallback = std::function<
    void(boost::beast::http::message_generator)
  >;

  MessageProcessor(std::shared_ptr<PathResolver> pathResolver);

  void operator()(RequestType&& request, ResponseCallback&& callback);

private:

  std::shared_ptr<PathResolver> pathResolver_;
  
};


}


#include "MessageProcessor.tpp"