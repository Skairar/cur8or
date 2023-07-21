#pragma once

#include <ranges>
#include <string_view>
#include <utility>
#include <list>


namespace utilities::adaptors {

class ParamPathAdaptor :
  public std::ranges::range_adaptor_closure<ParamPathAdaptor>
{
public:
  
  using PathSegment = std::pair<
    std::string_view,
    std::string_view
  >;

  using ListType = std::list<PathSegment>;

  ParamPathAdaptor(
    const char pathSeparator = '/',
    const char paramSign = '?'
  );

  ListType operator()(std::string_view path);

private:
  const char pathSeparator_,
             paramSign_;
};

}