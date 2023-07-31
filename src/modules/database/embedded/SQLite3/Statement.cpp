#include "Statement.hpp"

#include "ranges/Table.hpp"


namespace database::embedded::SQLite3{



Statement::Statement(std::shared_ptr<Instance> instance) noexcept
  : stmt_{nullptr},
    db_{instance}
{
  //empty
}



Statement::~Statement(){
  sqlite3_finalize(stmt_);
}



boost::leaf::result<Table> Statement::evaluate() noexcept{

  //likely unnecessary, might remove later unless I find need
  //to reuse statements
  sqlite3_reset(stmt_);
  int result = sqlite3_step(stmt_);
  switch(result){
    case SQLITE_DONE:
    case SQLITE_ROW:
      return Table{shared_from_this(), result};
      break;
    default:
      return boost::leaf::new_error(
        result,
        std::string{sqlite3_errstr(result)}
      );
  }

}



}//namespace database::embedded::SQLite3