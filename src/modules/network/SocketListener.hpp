#pragma once
#include <memory>
#include <functional>

#include <boost/system/error_code.hpp>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>


namespace network {


/**
 * @brief Basic listener and acceptor for new connections
 * 
 * Lightweight implementation of an asynchronous listener working on tcp
 * protocol. Currently doesn't contain any error resolution aside of stopping
 * itself on such. Will run forever or until the port it listens on closes.
 * The lifecycle is simple, after it's succesful creation function
 * acceptUpcoming should be ran, which starts a callback loop by invoking
 * run handler on socket, which invokes the callback passed on listener's
 * creation with newly connected socket, passing its ownership.
 * Afterwards invokes accept upcoming again.
 * 
 * 
 * TODO: More graceful break for the callback loop
 * TODO: Error handling and/or logging
 * 
 */
class SocketListener : public std::enable_shared_from_this<SocketListener>
{

public:

  /**
   * @brief Construct a new Socket Listener object
   * 
   * Immediately initiates the listening on passed endpoint, ceases in
   * case of endpoints, but leaves object in save state. 
   * 
   * @param executionContext asio's context on which listener is supposed
   * to dispatch its asynchronous operations
   * @param listenedSocket endpoint on which object is supposed to listen for
   * upcoming connections
   * @param connectionHandler a callable object that accepts a movable reference
   * to the socket with newly estabilished connection, its visible signature
   * should be equal to void(boost::asio::ip::tcp::socket&&).
   */
  SocketListener(
    boost::asio::io_context& executionContext,
    boost::asio::ip::tcp::endpoint listenedSocket,
    std::function<void(boost::asio::ip::tcp::socket&&)> &&connectionHandler
  ) noexcept;

  /**
   * @brief Starts the listener's async loop
   * 
   * Dispatches asynchronous accept operation on the context stack.
   * Creates new strand for each new connection. Binds its own shared pointer
   * to the operation, so its life is assured until loop gets broken on error.
   * Should be safe to call even in case of acceptor's failure during the
   * construction process.
   * 
   */
  void acceptUpcoming() noexcept;

private:

  /**
   * Immediately returns on error, breaking the callback loop. In case of
   * success, invokes the stored callback with newly created socket and
   * then calls acceptUpcoming to handle another connection.
   * 
   * @param errorCode 
   * @param socket 
   */
  void runHandlerOnSocket(
    boost::system::error_code errorCode,
    boost::asio::ip::tcp::socket socket
  ) noexcept;

  boost::asio::io_context &executionContext_;
  boost::asio::ip::tcp::acceptor connectionAcceptor_;
  std::function<void(boost::asio::ip::tcp::socket&&)> connectionHandler_;

};


}