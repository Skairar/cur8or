#pragma once

#include "JSON.hpp"


namespace database::embedded::SQLite3{


template <class FieldType>
void tag_invoke(
  const boost::json::value_from_tag&,
  boost::json::value& value,
  const Row<FieldType>& row
) noexcept{

  boost::json::array& values = value.emplace_array();
  values.reserve(row.size());
  for (auto field : row){
    values.emplace_back(boost::json::value_from(field));
  }

}


}