#pragma once

#include <string>
#include <string_view>
#include <filesystem>

#include <boost/beast/http/message.hpp>
#include <boost/beast/http/message_generator.hpp>
#include <boost/beast/http/string_body.hpp>


namespace network::http {


/**
 * @brief Helper class for turning request and protocol-less data obtained
 *  from its processing into a proper http response
 * 
 * Closes all the boilerplate setting up of http headers behind a quick to
 * use interface.
 * 
 */
class ResponseTemplate
{
public:

  using RequestType = boost::beast::http::request<
    boost::beast::http::string_body
  >;

  using ResponseType = boost::beast::http::message_generator;

  using StatusType = boost::beast::http::status;

  using PathType = std::filesystem::path;

  /**
   * @brief Construct a new Response Template object
   * 
   * @param request Request to be consumed in creation of response
   */
  ResponseTemplate(RequestType&& request) noexcept;

  /**
   * @brief Error response with additional detail
   * 
   * @param status Specifies the type of error
   * @param message Additional information about what went wrong
   * @return Ready to send ResponseType object
   */
  [[nodiscard]]
  ResponseType errorResponse(
    StatusType status,
    std::string_view message
  ) noexcept;

  /**
   * @brief Prepares the response based on the file from the disk
   * 
   * @param path Location of the file which will be loaded for the response 
   * @return Ready to send ResponseType object
   */
  [[nodiscard]]
  ResponseType fileResponse(
    PathType path
  ) noexcept;

  /**
   * @brief Response template for sending JSON data, mainly intended for
   * asynchronous requests
   * 
   * @param data JSON data serialized into string object, intended for move
   * @return Ready to send ResponseType object
   */
  [[nodiscard]]
  ResponseType dataResponse(
    std::string&& data
  ) noexcept;

private:

  RequestType request_;

};



}