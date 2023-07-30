#pragma once

#include <memory>
#include <functional>
#include <cstdint>

#include <boost/beast/http/message.hpp>
#include <boost/beast/http/message_generator.hpp>
#include <boost/beast/http/string_body.hpp>

#include "ResponseTemplate.hpp"

namespace network::http {

/**
 * @brief Wrapper for the request and all callbacks necessary for creating
 *  a response from processed data
 * 
 * Helper class for MessageProcessor and ResponseTemplate that makes sure 
 * both request and callback passed to a query are kept alive as long as 
 * they're needed. It is supposed to take full ownership of both request 
 * and callback. The three callback functions it employs pass request further, 
 * so only one of them should be invoked for any existing instance of this 
 * object and only once, it's intended to end its lifecycle afterwards.
 * 
 * Likely will be modfied in the future as its structure is heavily depended
 * on what is required by lower logic layers and implemented response templates.
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
  
  /**
   * @brief Construct a new Response Finalizer object
   * 
   * Both arguments are moved.
   * 
   * @param request Processed message
   * @param callback Functor used to pass the generated response to
   */
  ResponseFinalizer(
    RequestType&& request,
    ResponseCallback&& callback
  ) noexcept;

  /**
   * @brief Get the Request object
   * 
   * @return Reference to the request member
   */
  RequestType& getRequest() noexcept;

  /**
   * @brief Passes error response to the callback
   * 
   * Currently only generates bad request type of error.
   * 
   * @param message Additional details regarding the error
   */
  void errorResponseCallback(std::string_view message) noexcept;

  /**
   * @brief Passes file response to the callback 
   * 
   * @param path Location of the file used to generate the response
   */
  void fileResponseCallback(const PathType& path) noexcept;

  /**
   * @brief Passes data response to the callback
   * 
   * @param data Serialized JSON string, moved
   */
  void dataResponseCallback(std::string&& data) noexcept;

private:

  RequestType request_;

  ResponseCallback callback_;

};



}