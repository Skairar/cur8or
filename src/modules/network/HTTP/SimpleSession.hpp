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


/**
 * @brief Simple, lightweight HTTP session
 * 
 * Objects of this class provide the basic functionality in terms of
 * receiving requests and sending responses, with callback passed on
 * creation serving as a customization point.
 * 
 * I works in a callback loop, with startSession function serving as a
 * starting point. The general cycle looks as follows:
 * 
 * startSession()
 *       async call
 *      V
 * readStream() ---> procesRequest() --> loader_(request, callback)
 *      Ʌ     async call                           .
 *      |                                          . processing request, might
 *      | keep_alive=true                          . be happening in completely
 *      |                                          . different thread and module
 *      |                                          .
 * responseStatus() <--- sendResponse() <-- callback(response)
 *      |         async call
 *      | keep_alive=false || error
 *      V
 *  endSession()
 * 
 * The callback passed at creation, represented here as loader_, is a higher
 * order function that aside of requests receives another callable object
 * as it's argument, giving loader_ a complete freedom when it comes to 
 * processing request as long as it invokes back the callback with response
 * when it's finished. SimpleSession binds its own shared_pointer to the
 * response callback, so the caller can rest assured that the session object
 * will still be in memory when called back.
 * 
 * TODO: Error logging
 * 
 */
class SimpleSession : public std::enable_shared_from_this<SimpleSession>
{

public:

  using RequestType = boost::beast::http::request<
    boost::beast::http::string_body
  >;
  using ResponseCallback = std::function<
    void(boost::beast::http::message_generator)
  >;
  using ResponseLoader = std::function<
    void(RequestType&&, ResponseCallback&&)
  >;

  /**
   * @brief Construct a new Simple Session object
   * 
   * Assumes ownership over all passed arguments
   * 
   * @param socket accepted and estabilished connection on which session's
   *  data stream should run
   * @param loader a callable first order function that takes as it's arguments
   *  the pending request for processing and another function, intended for
   *  calling back with response constructed during said processing
   */
  SimpleSession(
    boost::asio::ip::tcp::socket&& socket,
    ResponseLoader&& loader
  ) noexcept;

  /**
   * @brief Starts the callback loop of the session
   * 
   * Aynchronously starts readStream function
   */
  void startSession() noexcept;

  /**
   * @brief Starts the reading from stream
   * 
   * Starts the asynchronous reading, with processRequest being called after
   * it's done
   */
  void readStream() noexcept;

  /**
   * @brief Callback to be invoked from asynchronous reading operation
   * 
   * Ends session in case of errors, invokes loader_ otherwise, passing
   * it function object made from binding sendResponse method with the
   * shared pointer of the session object
   * 
   * @param errorCode Status code returned by asynchronous read
   * @param bytes_transferred Required for signature, currently unused
   */
  void processRequest(
    boost::system::error_code errorCode,
    std::size_t bytes_transferred
  ) noexcept;

  /**
   * @brief Sends the response back to the peer
   * 
   * Shouldn't be called directly, use the callback passed to loader_
   * function object instead. Reads keep_alive from the message and binds it to 
   * the next callback. Starts asynchronous write which should invoke
   * responseStatus() function when finished. 
   * 
   * @param message Any response object from boost::beast library, gets passed
   *  through type erasure, function assumes the variable is to be moved
   */
  void sendResponse(
    boost::beast::http::message_generator&& message
  ) noexcept;

  /**
   * @brief Decides whether to continue session based on keep_alive and
   *  status returned by asynchronous writing
   * 
   * If there are no errors and keep_alive=true, invokes readStream()
   * 
   * @param keep_alive Flags whether to continue session
   * @param errorCode Status code returned by asynchronous write
   * @param bytesTransferred Required for signature, currently unused
   */
  void responseStatus(
    bool keep_alive,
    boost::system::error_code errorCode,
    std::size_t bytesTransferred
  ) noexcept;

  /**
   * @brief Safely close the session
   * 
   * Intended for calling when no error has occured and session ends because
   * of keep_alive being false
   */
  void endSession() noexcept;

private:

  boost::beast::tcp_stream stream_;
  boost::beast::flat_buffer buffer_;
  RequestType request_;             /**gets cleaned with every readStream()*/
  ResponseLoader loader_;

};



}