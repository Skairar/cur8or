#include "Instance.hpp"

#include "Statement.hpp"


namespace database::embedded::SQLite3{



boost::leaf::result<std::shared_ptr<Instance>> Instance::newInstance(
  const std::filesystem::path& path,
  int flags
) noexcept {

  std::shared_ptr<Instance> instance = std::make_shared<Instance>();
  int errorCode = instance->connect(path, flags);
  
  if (errorCode != SQLITE_OK){
    return boost::leaf::new_error(
      errorCode,
      std::string{sqlite3_errstr(errorCode)}
    );
  } else {
    return instance;
  }

}



boost::leaf::result<std::shared_ptr<Instance>> Instance::newInstance() noexcept{
  
  std::shared_ptr<Instance> instance = std::make_shared<Instance>();
  int errorCode = sqlite3_open(
    ":memory:",
    &(instance->db_)
  );
  
  if (errorCode != SQLITE_OK){
    return boost::leaf::new_error(
      errorCode,
      std::string{sqlite3_errstr(errorCode)}
    );
  } else {
    return instance;
  }

}


boost::leaf::result<std::shared_ptr<Statement>> Instance::prepareStatement(
  std::string_view sqlLine
) noexcept{
  
  std::shared_ptr<Statement> statement = std::make_shared<Statement>(
    shared_from_this()
  );
  int errorCode = sqlite3_prepare_v2(
    db_,
    sqlLine.data(),
    sqlLine.size(),
    &(statement->stmt_),
    nullptr
  );

  if (errorCode != SQLITE_OK){
    return boost::leaf::new_error(
      errorCode,
      std::string{sqlite3_errstr(errorCode)}
    );
  } else {
    return statement;
  }

}


Instance::~Instance(){
  //TO DO
  //Either implement some opaque static system behind the scenes that handles
  //the cases of sqlite3 instances not returning the SQLITE_OK upon first
  //sqlite3_close invokation or make absolutely sure such events are not
  //possible during runtime in the first place
  sqlite3_close(db_);
}


Instance::Instance()
  : db_{nullptr} 
{
  //empty
}


int Instance::connect(std::filesystem::path path, int flags) noexcept{
  
  int result = sqlite3_open_v2(
    path.string().c_str(), 
    &db_, 
    flags, 
    nullptr
  );
  if (result == SQLITE_OK){
    result = sqlite3_extended_result_codes(db_, 1);
  }
  return result;

}



}//namespace database::embedded::SQLite3