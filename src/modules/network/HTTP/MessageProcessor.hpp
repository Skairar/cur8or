#pragma once

#include <functional>
#include <memory>
#include <boost/beast/http/message.hpp>
#include <boost/beast/http/message_generator.hpp>
#include <boost/beast/http/string_body.hpp>


namespace network::http {


/**
 * @brief Functor serving as a frontend for message processing interface
 * 
 * It's invokable function is intended to be a frame for invoking all the
 * necessary HTTP processing functions and passing the data in more
 * generalized, protocol independent form to the deeper application logic,
 * as well as preparing specific callbacks ready to receive a generalized
 * data back and assemble it into a proper HTTP response passed back
 * to the session object.
 * 
 * In other words, it's role is to strip all the HTTP from the received
 * request, prepare a callbacks that will wrap the response data back into
 * HTTP headers and pass all of those to the logic layer.
 * 
 * @tparam PathResolver Type of an object passed to the constructor, serving
 *  as the interface for interacting with deeper layers of the application.
 * 
 * TODO: Implement logic for passing down and receiving header attributes,
 *  also some better handling of different http methods
 */
template <class PathResolver>
class MessageProcessor
  : public std::enable_shared_from_this<MessageProcessor<PathResolver>>
{
public:

  using RequestType = boost::beast::http::request<
    boost::beast::http::string_body
  >;

  using ResponseCallback = std::function<
    void(boost::beast::http::message_generator)
  >;

  /**
   * @brief Construct a new Message Processor object
   * 
   * The type of passed argument is used as the template parameter
   * for the class.
   * 
   * @param pathResolver Bridge object serving for exchanging data with
   *  deeper layers of application
   */
  MessageProcessor(std::shared_ptr<PathResolver> pathResolver) noexcept;

  /**
   * @brief Primary callable interface of the functor
   * 
   * Assumes both arguments are to be moved.
   * 
   * @param request Message to be processed
   * @param callback Functor to invoke with prepared response
   */
  void operator()(RequestType&& request, ResponseCallback&& callback) noexcept;

private:

  std::shared_ptr<PathResolver> pathResolver_;
  
};


}


#include "MessageProcessor.tpp"