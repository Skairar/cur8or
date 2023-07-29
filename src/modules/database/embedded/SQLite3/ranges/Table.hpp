#pragma once

#include <memory>
#include <ranges>
#include <iterator>
#include <utility>

#include <sqlite3.h>


#include "Row.hpp"
#include "Field.hpp"

namespace database::embedded::SQLite3{


class KeyField;
class ValueField;
template <class FieldType>
class Row;



class Statement;

/*
  Represents a table
*/
class Table : public std::ranges::view_interface<Table>
{

  friend class Statement;

public:

  class Iterator;

  class Sentinel;

  Table(std::shared_ptr<Statement> statement, int result) noexcept;

public:

  [[nodiscard]]
  Row<KeyField> columnHeaders() const noexcept;

  [[nodiscard]]
  Row<ValueField> currentRow() const noexcept;

  [[nodiscard]]
  std::pair<int, std::string> lastOperationResult() const noexcept;

  int nextRow() const noexcept;

  [[nodiscard]]
  Iterator begin() const noexcept;

  [[nodiscard]]
  Sentinel end() const noexcept;

private:

  std::shared_ptr<Statement> statementGuard_;

  sqlite3_stmt* stmt_;

  mutable int lastOpResult_;

  int columnCount_;

};


class Table::Iterator
{

  friend class Table::Sentinel;

public:

  using iterator_category = std::input_iterator_tag;
  using value_type = Row<ValueField>;
  using pointer = std::unique_ptr<Row<ValueField>>;
  using reference = value_type&;

  Iterator(const Table* table) noexcept;

  Iterator& operator++() noexcept;

  pointer operator->() const noexcept;

  value_type operator*() const noexcept;

private:

  const Table* table_;

};


class Table::Sentinel
{
public:

  bool operator==(Iterator iter) const noexcept;

};


}