#pragma once

#include <boost/json.hpp>

#include "../ranges/Table.hpp"


namespace database::embedded::SQLite3{


namespace JSON{


class ValueFieldVisitor
{
public:

  ValueFieldVisitor(boost::json::value& value) noexcept;

  void operator()(const int value) noexcept;
  void operator()(const double value) noexcept;
  void operator()(const char* str, const unsigned int count) noexcept;
  void operator()() noexcept;

private:

  boost::json::value& value_;

};


}


void tag_invoke(
  const boost::json::value_from_tag&,
  boost::json::value& value,
  const Table& table
) noexcept;


template <class FieldType>
void tag_invoke(
  const boost::json::value_from_tag&,
  boost::json::value& value,
  const Row<FieldType>& row
) noexcept;


void tag_invoke(
  const boost::json::value_from_tag&,
  boost::json::value& value,
  const KeyField& field
) noexcept;


void tag_invoke(
  const boost::json::value_from_tag&,
  boost::json::value& value,
  const ValueField& field
) noexcept;


}


#include "JSON.tpp"