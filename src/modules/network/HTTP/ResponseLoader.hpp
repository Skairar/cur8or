#pragma once

#include <string_view>
#include <filesystem>
#include <functional>

#include <boost/beast/http/message_generator.hpp>



namespace network::http {


namespace http = boost::beast::http;


std::string_view fileMimeType(const std::filesystem::path &filePath) noexcept;

template <class ResourceProvider>
class ResponseLoader
{

  using RequestType = http::request<http::string_body>;

  using ResponseCallback = std::function<void(http::message_generator&&)>;

public:

  ResponseLoader(std::shared_ptr<ResourceProvider> resourceProvider);

  void generateResponse(
    RequestType&& request,
    ResponseCallback&& onResponseReadyCallback
  );

private:

  std::shared_ptr<ResourceProvider> resourceProvider_;
  
  http::message_generator errorResponse(http::status status, std::string_view message);

};


}


#include "ResponseLoader.tpp"