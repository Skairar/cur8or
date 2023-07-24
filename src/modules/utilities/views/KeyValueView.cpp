#include "KeyValueView.hpp"

#include <algorithm>


namespace utilities::views {


KeyValueView::KeyValueView(
  std::string_view mappedString,
  const char keyValueSeparator,
  const char pairSeparator
) : keyValueSeparator_{keyValueSeparator},
    pairSeparator_{pairSeparator},
    map_{mapString(mappedString)}
{
  //empty
}


KeyValueView::iterator KeyValueView::begin() const noexcept {
  return map_.begin();
}


KeyValueView::iterator KeyValueView::end() const noexcept {
  return map_.end();
}


bool KeyValueView::empty() const noexcept {
  return map_.empty();
}


KeyValueView::size_type KeyValueView::size() const noexcept {
  return map_.size();
}


KeyValueView::size_type KeyValueView::count(
  const KeyValueView::key_type& key
) const noexcept {
  return map_.count(key);
}


KeyValueView::iterator KeyValueView::find(
  const KeyValueView::key_type& key
) const noexcept {
  return map_.find(key);
}


bool KeyValueView::contains(const KeyValueView::key_type& key) const noexcept {
  return map_.contains(key);
}


KeyValueView::IteratorPair KeyValueView::equal_range(
  const KeyValueView::key_type& key
) const noexcept {
  return map_.equal_range(key);
}


KeyValueView::MapType KeyValueView::mapString(std::string_view mappedString) {
  MapType map;

  std::string_view str = mappedString;

  {
    /*
      The loop will iterate [count of pair separators + 1] times.
    */
    unsigned int segments = std::count(
      str.begin(), 
      str.end(), 
      pairSeparator_
    );
    std::size_t pos = 0;
    while (0 <= segments) {
      std::size_t delim = str.find(pairSeparator_, pos);
      if (delim > 0) {
        std::string_view pair = str.substr(pos, delim);
        std::size_t sep = pair.find(keyValueSeparator_);
        if (sep != 0 || delim != 1) {
          map.insert({
            str.substr(0, sep),
            str.substr(sep + 1)
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