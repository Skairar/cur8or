#include "ParamPathAdaptor.hpp"

#include <algorithm>

namespace utilities::adaptors {

ParamPathAdaptor::ParamPathAdaptor(
  const char pathSeparator,
  const char paramSign
) : pathSeparator_{pathSeparator},
    paramSign_{paramSign}
{
  //empty
}


ParamPathAdaptor::ListType ParamPathAdaptor::operator()(std::string_view path) {
  ListType list;
  int segments = std::count(
    path.begin(),
    path.end(),
    pathSeparator_
  );
  std::size_t pos = path.find(pathSeparator_) + 1;
  while (segments > 0) {
    std::size_t delim = path.find(pathSeparator_, pos) - pos;
    std::string_view pair = path.substr(pos, delim);
    std::size_t sep = pair.find(paramSign_);
    list.emplace_back(
      pair.substr(0, sep),
      pair.substr(sep + 1, (sep==path.npos)?0:pair.npos)
    );
    pos += delim + 1;
    --segments;
  } 
  return list;
}

}