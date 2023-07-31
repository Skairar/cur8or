#pragma once

#include <string_view>
#include <filesystem>


namespace network::http::helpers {


/**
 * @brief Returns the mime type string for the file on given location
 * 
 * @param path Location of file
 * @return Mime type string for file 
 */
std::string_view fileMimeType(
  const std::filesystem::path& path
) noexcept;


}