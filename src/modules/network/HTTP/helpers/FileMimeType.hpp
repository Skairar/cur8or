#pragma once

#include <string_view>
#include <filesystem>


namespace network::http::helpers {


std::string_view fileMimeType(const std::filesystem::path& path) noexcept;


}