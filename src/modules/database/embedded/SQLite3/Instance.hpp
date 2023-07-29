#pragma once

#include <memory>
#include <filesystem>
#include <boost/leaf/result.hpp>

#include <sqlite3.h>


namespace database::embedded::SQLite3{

class Statement;

class Instance : public std::enable_shared_from_this<Instance>
{
public:

  [[nodiscard]]
  static boost::leaf::result<std::shared_ptr<Instance>> newInstance(
    const std::filesystem::path& path,
    int flags
  ) noexcept;

  //in memory database, for testing purposes
  [[nodiscard]]
  static boost::leaf::result<std::shared_ptr<Instance>> newInstance() noexcept;

  Instance();

  ~Instance();

  //Might consider creating a separate object to represent entire
  //SQL scripts, until then processing statements one object per 
  //semicolon ended statements is the only allowed way.
  //Separating viewing-only statements from those actively modifying
  //the database also is in consideration, alas it's a subject
  //largely intersecting with the former as far as I'm aware.
  boost::leaf::result<std::shared_ptr<Statement>>
    prepareStatement(std::string_view statement) noexcept;

private:

  int connect(std::filesystem::path path, int flags) noexcept;

  sqlite3* db_;

};


}