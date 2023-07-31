#pragma once

#include <memory>
#include <filesystem>
#include <boost/leaf/result.hpp>

#include <sqlite3.h>


namespace database::embedded::SQLite3{



class Statement;



/**
 * @brief SQLite3 database connection
 * 
 * Pseudo-RAII wrapper for sqlite3 database "connection". New instances
 * should be created with provided factory methods, which properly initialize
 * the database handle.
 * 
 * Minimalistic version for now, will get expanded to cover more of the
 * necessary SQLite3 functionality.
 * 
 * Error reporting facilities around the library are implemented with the
 * use of boost::leaf. All instances are created and passed around in 
 * std::shared_ptr objects and shouldn't be used without them.
 * 
 * Not using constructors for object creation and depending on dedicated
 * functions for that instead is the general rule in this implementation. 
 * 
 */
class Instance : public std::enable_shared_from_this<Instance>
{

public:

  /**
   * @brief Opens new instance for SQLite3
   * 
   * Currently uses flags straight from sqlite3.h header, might be changed
   * in the future.
   * 
   * @param path Location of the SQLite3 database file on the filesystem
   * @param flags Flags used for opening the connection
   * @return boost::leaf result object that depending on whether operation
   *  was succesful will contain either a newly opened Instance or Error
   *  describing what went wrong
   */
  [[nodiscard]]
  static boost::leaf::result<std::shared_ptr<Instance>> newInstance(
    const std::filesystem::path& path,
    int flags
  ) noexcept;

  /**
   * @brief Opens in memory instance
   * 
   * Similar to the other overload, except it's fully in-memory database
   * with no representation on the drive, opened with default flags.
   * Intended for testing.
   * 
   * @return Boost::leaf result with either new instance or error
   */
  [[nodiscard]]
  static boost::leaf::result<std::shared_ptr<Instance>> newInstance() noexcept;

  /**
   * @brief Creates empty instance, shouldn't be called on its own
   * 
   * Use the static functions for making new instances instead. Public only
   * because of some issues I had with initialization previously, will likely
   * private when I find the workaround.
   * 
   */
  Instance();

  /**
   * @brief Closes the connection
   * 
   */
  ~Instance();

  /**
   * @brief Creates new prepared statement.
   * 
   * @param statement Single line of SQL, terminated with semicolon.
   * @return Pointer to newly allocated prepared statement on success, error
   *  code and message on failure.
   */
  boost::leaf::result<std::shared_ptr<Statement>>
    prepareStatement(std::string_view statement) noexcept;

private:

  /**
   * @brief Opening of new connection, for internal use only
   * 
   * @param path 
   * @param flags 
   * @return Sqlite3 error code
   */
  int connect(std::filesystem::path path, int flags) noexcept;

  sqlite3* db_;

};



}