#pragma once

#include <functional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <filesystem>

#include "VirtualPathResolver.hpp"



namespace logic::vpath{

class VirtualPathResolver;

/*
  Objects of this type represent a an environment to process a single
  vpath command.
*/
class Query
{
  
  friend class VirtualPathResolver;

public:

  using ParameterMap = std::unordered_map<
    std::string_view,
    std::string_view
  >;

  using QueryProgram = std::function<void(Query&)>;

  using ErrorCallback = std::function<void(std::string_view)>;

  using FileCallback = std::function<void(std::filesystem::path)>;

  using DataCallback = std::function<void(std::string&&)>;

  void run();

  [[nodiscard]]
  std::string_view getTarget() const noexcept;

  void setTarget(std::string_view) noexcept;

  [[nodiscard]]
  std::string_view getData() const noexcept;

  void setData(std::string_view data) noexcept;

  [[nodiscard]]
  ParameterMap& getParameterMap() noexcept;

  void setParameterMap(ParameterMap&& parameterMap) noexcept;

  void setErrorCallback(ErrorCallback&& callback) noexcept;

  void returnError(std::string_view) const noexcept;

  void setFileCallback(FileCallback&& callback) noexcept;

  void returnFile(std::filesystem::path path) const noexcept;

  void setDataCallback(DataCallback&& callback) noexcept;

  void returnData(std::string&& data) const noexcept;

private:

  std::string_view target_,
                   data_;
  
  ParameterMap parameterMap_;

  QueryProgram query_;

  ErrorCallback returnError_;

  FileCallback returnFile_;

  DataCallback returnData_;

  Query() = default;

  void setQueryProgram(QueryProgram callback) noexcept;

};



}