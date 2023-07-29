#pragma once

#include "Field.hpp"

#include <cstring>


namespace database::embedded::SQLite3{


template <class Visitor>
void ValueField::operator()(Visitor visitor) const noexcept{
  switch(sqlite3_column_type(const_cast<sqlite3_stmt*>(stmt_), column_)){
    case SQLITE_INTEGER:
      visitor(
        sqlite3_column_int(const_cast<sqlite3_stmt*>(stmt_), column_)
      );
      return;
    case SQLITE_FLOAT:
      visitor(
        sqlite3_column_double(const_cast<sqlite3_stmt*>(stmt_), column_)
      );
      return;
    case SQLITE_TEXT:
      visitor(
        reinterpret_cast<const char*>(
          sqlite3_column_text(const_cast<sqlite3_stmt*>(stmt_), column_)
        ),
        sqlite3_column_bytes(const_cast<sqlite3_stmt*>(stmt_), column_)
      );
      return;
    [[unlikely]] case SQLITE_BLOB:
      {
        //Omitted for now, don't really have any use case for blobs
        //as of now and handling them without proper context in mind
        //is a bit quirky.
        constexpr const char* str = ":blob value not supported:";
        visitor(str, strlen(str));
        return;
      }
    case SQLITE_NULL:
      visitor();
      return;
    [[unlikely]] default:
      //not possible at the time of writing
      return;
  }
}


}