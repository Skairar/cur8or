#pragma once

#include <memory>
#include <iterator>
#include <ranges>
#include <variant>

#include <boost/leaf/result.hpp>

#include <sqlite3.h>


namespace database::embedded::SQLite3{

class Instance;
class Table;

/*
  Represents a single sql statement that was compiled by the database
  engine and is ready to be evaluated. Currently represents all possible
  statements, both those that return a Table and those that change the
  insides of the database, which might change in the future.

  TO DO
  Variable binding
*/
class Statement : public std::enable_shared_from_this<Statement>
{

  friend class Instance;
  friend class Table;

public:
 
  Statement(std::shared_ptr<Instance> instance) noexcept;

  ~Statement();

  [[nodiscard]]
  boost::leaf::result<Table> evaluate() noexcept;

private:

  sqlite3_stmt* stmt_;

  std::shared_ptr<Instance> db_;

};








}