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

/**
 * @brief Prepared statement ready for evaluation
 * 
 * Instances of this class should be only created with use of Instance's
 * prepareStatement function.
 * 
 * 
 */
class Statement : public std::enable_shared_from_this<Statement>
{

  friend class Instance;
  friend class Table;

public:
 
  /**
   * @brief Construct a new Statement object
   * 
   * @param instance Database instance which compiled the statement
   */
  Statement(std::shared_ptr<Instance> instance) noexcept;

  /**
   * @brief Destroy the Statement object
   * 
   */
  ~Statement();

  /**
   * @brief Evaluates the statement
   * 
   * Returns table for all possible queries, it will just be empty for
   * non-select statemets. Returned table also contains the result code of
   * the last operation.
   * 
   * The statement can be evaluated many times, but each time it will
   * invalidate the previously returned table objects.
   * 
   * @return Table object which, depending on the operation, might be empty.
   */
  [[nodiscard]]
  boost::leaf::result<Table> evaluate() noexcept;

private:

  sqlite3_stmt* stmt_;

  std::shared_ptr<Instance> db_;

};








}