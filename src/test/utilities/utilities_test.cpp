#define BOOST_TEST_MODULE Utilities Test
#include <boost/test/included/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>

#include "utilities/adaptors/KeyValueAdaptor.hpp"
#include "utilities/adaptors/ParamPathAdaptor.hpp"

#include <unordered_map>
#include <string_view>
#include <iostream>
#include <utility>

using MapType = utilities::adaptors::KeyValueAdaptor::MapType;

using KeyValueDataset = std::unordered_map<
  std::string_view,
  MapType
>;

KeyValueDataset keyValueDatasetGen() {
  using namespace std::literals;
  static const KeyValueDataset dataset{
    {"key1=value1&key2=value2&key3=value3"sv,
      {
        {"key1"sv, "value1"sv},
        {"key2"sv, "value2"sv},
        {"key3"sv, "value3"sv}
      }
    },
    {"key1=value1&keyvalue2&key3=value3"sv,
      {
        {"key1"sv, "value1"sv},
        {"keyvalue2"sv, "keyvalue2"sv},
        {"key3"sv, "value3"sv}
      }
    },
    {"&key1=value1&key2=value2&key3=value3"sv,
      {
        {"key1"sv, "value1"sv},
        {"key2"sv, "value2"sv},
        {"key3"sv, "value3"sv}
      }
    },
    {"key1=value1&&key2=value2"sv,
      {
        {"key1"sv, "value1"sv},
        {"key2"sv, "value2"sv}
      }
    },
    {"=&=value1&key2=value2=value2&=&=&&key3==value3&&&=&"sv,
      {
        {""sv, "value1"sv},
        {"key2"sv, "value2=value2"sv},
        {"key3"sv, "=value3"sv}
      }
    },
  };
  return dataset;
}

/*
  Quick solution for now, the test framework parses all the sample
  dataset with boost logger and it tends to have issues with std containers.

  TO DO: Either implement some wrapper types or find another way to define
  tests that doesn't require all the samples to be immediately printable
*/
typedef std::pair<
  const std::basic_string_view<char>,
  std::unordered_multimap<
    std::basic_string_view<char>,
    std::basic_string_view<char>
  >
> pair;
BOOST_TEST_DONT_PRINT_LOG_VALUE(pair)

BOOST_DATA_TEST_CASE(
  KeyValueViewTest, 
  boost::unit_test::data::make(keyValueDatasetGen()),
  sample
)
{
  using namespace utilities::adaptors;
  MapType expectedRes = sample.second;
  std::cout << "Test for: " << sample.first << std::endl;
  for (auto res : sample.first | KeyValueAdaptor{}) {
    auto [iter, last] = expectedRes.equal_range(res.first);
    bool erased = false;
    for (;iter != last; ++iter) {
      if (res.second == iter->second) {
        expectedRes.erase(iter);
        erased = true;
        break;
      }
    }
    std::cout << res.first << ", " << res.second << std::endl;
    BOOST_TEST(erased == true);
  }
  for (auto i : expectedRes) {
    std::cout << i.first << ", " << i.second << std::endl;
  }
  BOOST_TEST(expectedRes.empty() == true);
}




using ListType = utilities::adaptors::ParamPathAdaptor::ListType;
using ParamPathDataset = std::unordered_map<
  std::string_view,
  ListType
>;

ParamPathDataset paramPathDatasetGen() {
  using namespace std::literals;
  static const ParamPathDataset dataset{
    {"/target1?arguments1/target2?arguments2/target3?arguments3"sv,
      {
        {"target1"sv, "arguments1"sv},
        {"target2"sv, "arguments2"sv},
        {"target3"sv, "arguments3"sv}
      }
    },
    {"/target1?key1=value1/target2?arguments/target3/"sv,
      {
        {"target1"sv, "key1=value1"sv},
        {"target2"sv, "arguments"sv},
        {"target3"sv, ""},
        {""sv, ""sv}
      }
    },
    {"suffix/target1?arguments1////target2?arguments2/target3?arguments3"sv,
      {
        {"target1"sv, "arguments1"sv},
        {""sv, ""sv},
        {""sv, ""sv},
        {""sv, ""sv},
        {"target2"sv, "arguments2"sv},
        {"target3"sv, "arguments3"sv}
      }
    },
    {"/target1/target2?arguments2/target3?///"sv,
      {
        {"target1"sv, ""sv},
        {"target2"sv, "arguments2"sv},
        {"target3"sv, ""sv},
        {""sv, ""sv},
        {""sv, ""sv},
        {""sv, ""sv}
      }
    }
  };

  return dataset;
}


typedef std::_List_iterator<
  std::pair<
    std::basic_string_view<char>,
    std::basic_string_view<char>
  >
> list_iter;
BOOST_TEST_DONT_PRINT_LOG_VALUE(list_iter)
typedef std::pair<
  const std::basic_string_view<char>,
  std::__cxx11::list<
    std::pair<
      std::basic_string_view<char>,
      std::basic_string_view<char>
    >
  >
> pair2;
BOOST_TEST_DONT_PRINT_LOG_VALUE(pair2)


BOOST_DATA_TEST_CASE(
  ParamPathAdaptorTest, 
  boost::unit_test::data::make(paramPathDatasetGen()),
  sample
)
{
  using namespace utilities::adaptors;
  std::cout << "Test for: " << sample.first << std::endl;
  ListType results = sample.first | ParamPathAdaptor{},
           expected = sample.second;
  auto i = results.begin(),
       j = expected.begin();
  while ((i != results.end()) && (j != expected.end())) {
    std::cout << "["
              << i -> first
              << ", "
              << i -> second
              << "] ? ["
              << j -> first
              << ", "
              << j -> second
              << "]"
              << std::endl;
    BOOST_TEST(i->first == j->first);
    BOOST_TEST(i->second == j->second);
    ++i;
    ++j;
  }
  BOOST_TEST(i == results.end());
  BOOST_TEST(j == expected.end());
}