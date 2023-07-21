#include "ResponseLoader.hpp"

#include <unordered_map>
#include <string_view>
#include <filesystem>

namespace network::http {



std::string_view fileMimeType(const std::filesystem::path &filePath) noexcept
{
  using namespace std::literals;

  static const std::unordered_map<
    std::string_view,
    std::string_view
  > extensionMapping = {
      {".htm"sv, "text/html"sv},
      {".html"sv, "text/html"sv},
      {".php"sv, "text/html"sv},
      {".css"sv, "text/css"sv},
      {".txt"sv, "text/plain"sv},
      {".js"sv, "application/javascript"sv},
      {".json"sv, "application/json"sv},
      {".xml"sv, "application/xml"sv},
      {".swf"sv, "application/x-shockwave-flash"sv},
      {".flv"sv, "video/x-flv"sv},
      {".png"sv, "image/png"sv},
      {".jpe"sv, "image/jpeg"sv},
      {".jpeg"sv, "image/jpeg"sv},
      {".jpg"sv, "image/jpeg"sv},
      {".gif"sv, "image/gif"sv},
      {".bmp"sv, "image/bmp"sv},
      {".ico"sv, "image/vnd.microsoft.icon"sv},
      {".tiff"sv, "image/tiff"sv},
      {".tif"sv, "image/tiff"sv},
      {".svg"sv, "image/svg+xml"sv},
      {".svgz"sv, "image/svg+xml"sv},
      {""sv, "application/text"sv}
  };

  auto iter = extensionMapping.find(filePath.extension().string());

  return (iter != extensionMapping.end()) ? iter->second : "application/text"sv;
}


}