#pragma once

#include <string>

#include <sqlite3.h>

#include "Row.hpp"


namespace database::embedded::SQLite3{


template <class FieldType>
class Row;

class KeyField
{

  friend class Row<KeyField>;
  //friend class Row<KeyField>::Iterator;

public:

  KeyField(const sqlite3_stmt* stmt, const int column) noexcept;

  std::string operator()() const noexcept;

private:

  const sqlite3_stmt* stmt_;

  const int column_;

};


class ValueField
{

  friend class Row<ValueField>;
  //friend class Row<ValueField>::Iterator;

public:

  ValueField(const sqlite3_stmt* stmt, const int column) noexcept;

  template <class Visitor>
  void operator()(Visitor visitor) const noexcept;

private:

  const sqlite3_stmt* stmt_;

  const int column_;

};


}


#include "Field.tpp"