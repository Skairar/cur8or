#include "Field.hpp"

namespace database::embedded::SQLite3{


KeyField::KeyField(
  const sqlite3_stmt* stmt,
  const int column
) noexcept
 : stmt_{stmt},
   column_{column}
{
  //empty
}


std::string KeyField::operator()() const noexcept{
  return sqlite3_column_name(const_cast<sqlite3_stmt*>(stmt_), column_);
}


ValueField::ValueField(const sqlite3_stmt* stmt, const int column) noexcept
 : stmt_{stmt},
   column_{column}
{
  //empty
}


}