#pragma once

#include <string_view>
#include <unordered_map>
#include <utility>
#include <ranges>


namespace utilities::views {


/*
  Helper class with a purpose of indexing a character string of
  intended form {key1=value1&key2=value2&key3=value3...}, where
  '&' stands for pair separator and '=' - key-value separator.
  Empty pairs in which both key and value are empty strings are
  not allowed, thus guaranting that the number of pairs stored
  will be equal to the number of non empty string produced by
  i.e. javascript's split function invoked with pair separator.
  The class uses std::unordered_multimap to store its pairs, thus
  allowing for multiple instances of the same key appearing.
  Empty keys are allowed as long as their value is not empty.
  However, in case of non empty key having an empty value, the
  key becomes its value itself, so another index produced from
  the same string but with its key and values swapped won't be
  equal to the key-value swap of index produced from the original
  string.

  The class doesn't own the string it indexes and its built-in
  multimap only stores std:string_view type of objects that point
  to memory with the string used in constructor, so it's necesserary
  to keep the orignal string alive as long as the associated object
  of this class is used.

  NOTE:
  As of now, the "pairs" with more than one key-value separator
  are allowed and the additional separators will be interpreted
  as part of value, with only the first separator being consumed.
  It's mainly because I couldn't really decide what do I want from
  it in such peculiar case, might change it in the future when I
  get some actual use for this form.
*/
class KeyValueView : public std::ranges::view_interface<KeyValueView>
{

  using MapType = std::unordered_multimap<
    std::string_view,
    std::string_view
  >;

  //several typedefs for some stl typenames, kept the original
  //naming convention
  using key_type = MapType::key_type;
  using iterator = MapType::const_iterator;
  using size_type = MapType::size_type;

  using IteratorPair = std::pair<iterator, iterator>;

public:

  KeyValueView(
    std::string_view sourceString,
    const char keyValueSeparator = '=',
    const char pairSeparator = '&'
  );

  [[nodiscard]]
  iterator begin() const noexcept;

  [[nodiscard]]
  iterator end() const noexcept;

  [[nodiscard]]
  bool empty() const noexcept;

  [[nodiscard]]
  size_type size() const noexcept;

  [[nodiscard]]
  size_type count(const key_type& key) const noexcept;

  [[nodiscard]]
  iterator find(const key_type& key) const noexcept;

  [[nodiscard]]
  bool contains(const key_type& key) const noexcept;

  [[nodiscard]]
  IteratorPair equal_range(const key_type& key) const noexcept;

private:

  const char keyValueSeparator_,
             pairSeparator_; 
  const MapType map_;

  MapType mapString(std::string_view mappedString);

};


}