#include "Table.hpp"

#include "../Statement.hpp"
#include "Row.hpp"


namespace database::embedded::SQLite3{


Table::Table(std::shared_ptr<Statement> statement, int result) noexcept
 : statementGuard_{statement},
   stmt_{statementGuard_->stmt_},
   lastOpResult_{result},
   columnCount_{sqlite3_column_count(const_cast<sqlite3_stmt*>(stmt_))}
{
  //empty
}


Row<KeyField> Table::columnHeaders() const noexcept{
  return Row<KeyField>{stmt_, columnCount_};
}


Row<ValueField> Table::currentRow() const noexcept{
  return Row<ValueField>{stmt_, columnCount_};
}


int Table::nextRow() const noexcept{
  lastOpResult_ = sqlite3_step(const_cast<sqlite3_stmt*>(stmt_));
  return lastOpResult_;
}

std::pair<int, std::string> Table::lastOperationResult() const noexcept{
  return std::make_pair(
    lastOpResult_, 
    std::string{sqlite3_errstr(lastOpResult_)}
  );
}


Table::Iterator Table::begin() const noexcept{
  return Iterator{this};
}


Table::Sentinel Table::end() const noexcept{
  return Sentinel{};
}


Table::Iterator::Iterator(const Table* table) noexcept
 : table_{table}
{
  //empty
}


Table::Iterator& Table::Iterator::operator++() noexcept{
  table_->nextRow();
  return *this;
}


Table::Iterator::pointer Table::Iterator::operator->() const noexcept{
  return std::make_unique<value_type>(table_->currentRow());
}


Table::Iterator::value_type Table::Iterator::operator*() const noexcept{
  return table_->currentRow();
}


bool Table::Sentinel::operator==(Table::Iterator iter) const noexcept{
  return (iter.table_->lastOpResult_ != SQLITE_ROW);
}


}