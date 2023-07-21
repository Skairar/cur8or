#pragma once

#include <string_view>
#include <unordered_map>
#include <utility>
#include <ranges>


namespace utilities::adaptors {


class KeyValueAdaptor :
  public std::ranges::range_adaptor_closure<KeyValueAdaptor>
{

public:

  using MapType = std::unordered_multimap<
    std::string_view,
    std::string_view
  >;

  KeyValueAdaptor(
    const char keyValueSeparator = '=',
    const char pairSeparator = '&'
  );

  MapType operator()(std::string_view mappedString);

private:
  const char keyValueSeparator_,
             pairSeparator_; 

};


}