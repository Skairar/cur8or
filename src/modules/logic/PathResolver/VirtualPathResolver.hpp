#pragma once

#include <functional>
#include <unordered_map>
#include <string_view>

#include "Query.hpp"

namespace logic::vpath {

class Query;
/*
  A frontend of a bridge connecting application's frontend with backend.
  Currently a very minimalistic implementation that only supports
  most basic of planned operations.
  The intended future implementation is planned to support more advanced
  validation of requested operations with their arguments and seamless
  operation piping.
  Depending on how much complicated operations would be implemented in the
  future, the addition of its individual asio::io_context might be considered.
  At the same time hovewer, keeping it as a simple bridge that stays read
  only and dispatches heavier operations during runtime would be ideal to keep
  everything multithread friendly.
*/
class VirtualPathResolver
{
public:

  using CallbackType = std::function<void(Query&)>;

  using VirtualPathMap = std::unordered_map<
    std::string_view,
    CallbackType
  >;

  //temporary
  VirtualPathResolver() = default;

  void registerCallback(std::string_view vPath, CallbackType&& callback);

  Query newQuery(std::string_view target);

  bool supportedOperation(std::string_view);

private:

  VirtualPathMap pathMap_;

};


}