#include "JSON.hpp"


namespace database::embedded::SQLite3{


namespace JSON{


ValueFieldVisitor::ValueFieldVisitor(boost::json::value& value) noexcept
 : value_{value}
{
  //empty
}


void ValueFieldVisitor::operator()(const int value) noexcept{
  value_.emplace_int64() = value;
}


void ValueFieldVisitor::operator()(const double value) noexcept{
  value_.emplace_double() = value;
}


void ValueFieldVisitor::operator()(
  const char* str,
  const unsigned int count
) noexcept{
  value_.emplace_string() = std::move(std::string{
    str, count
  });
}


void ValueFieldVisitor::operator()() noexcept{
  value_.emplace_null();
}


}


void tag_invoke(
  const boost::json::value_from_tag&,
  boost::json::value& value,
  const Table& table
) noexcept{
  boost::json::object& object = value.emplace_object();
  object["header"] = boost::json::value_from(table.columnHeaders());
  boost::json::array& values = object["values"].emplace_array();
  for (auto row : table){
    values.emplace_back(boost::json::value_from(row));
  }
  auto [statusCode, message] = table.lastOperationResult();
  boost::json::object& status = object["status"].emplace_object();
  status.emplace("statusCode", statusCode);
  status.emplace("message", std::move(message));

}


void tag_invoke(
  const boost::json::value_from_tag&,
  boost::json::value& value,
  const KeyField& field
) noexcept{
  value.emplace_string() = std::move(field());
}


void tag_invoke(
  const boost::json::value_from_tag&,
  boost::json::value& value,
  const ValueField& field
) noexcept{
 field(JSON::ValueFieldVisitor{value});
}

  
}