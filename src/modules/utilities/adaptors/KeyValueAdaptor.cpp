#include "KeyValueAdaptor.hpp"

#include <algorithm>


namespace utilities::adaptors {


KeyValueAdaptor::KeyValueAdaptor(
  const char keyValueSeparator,
  const char pairSeparator
) : keyValueSeparator_{keyValueSeparator},
    pairSeparator_{pairSeparator}
{
  //empty
}


KeyValueAdaptor::MapType KeyValueAdaptor::operator()(
  std::string_view str
) {
  MapType map;
  {
    /*
      The loop will iterate [count of pair separators + 1] times.
    */
    int segments = std::count(
      str.begin(), 
      str.end(), 
      pairSeparator_
    );
    std::size_t pos = 0;
    while (0 <= segments) {
      std::size_t delim = str.find(pairSeparator_, pos) - pos;
      if (delim > 0) {
        std::string_view pair = str.substr(pos, delim);
        std::size_t sep = pair.find(keyValueSeparator_);
        if ((sep != 0 || delim != 1) && pair.size() != 0) {
          map.insert({
            pair.substr(0, sep),
            pair.substr(sep + 1)
          });
        }
      }
      pos += delim + 1;
      --segments;
    }
  }
  return map;
}


}