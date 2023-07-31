#pragma once

#include <functional>
#include <unordered_map>
#include <string_view>

#include "Query.hpp"


namespace logic::vpath {



class Query;
/**
 * @brief A frontend of a bridge connecting application's user interface
 *  with its functionality
 * 
 * Functionalities are represented to the frontend as virtual paths, mapped to
 * the sequences of proper functor invocations. Object of this class
 * provides an easy interface for adding new operations on any point during
 * its lifetime and for checking the validity of such operations.
 * 
 * Indended as a simple facade to the deeper program logic, invokable by
 * higher layers of application.
 * 
 * Currently a very minimalistic implementation that only supports most basic 
 * of planned operations. The intended future implementation is planned to 
 * support more advanced validation of requested operations with their 
 * arguments and seamless operation piping. Depending on how much complicated 
 * operations would be implemented in the future, the addition of its 
 * individual asio::io_context might be considered. At the same time hovewer, 
 * keeping it as a simple bridge that stays read only and dispatches heavier 
 * operations during runtime would be ideal to keep everything multithread 
 * friendly.
 * 
 */
class VirtualPathResolver
{
public:

  using CallbackType = std::function<void(Query&)>;

  using VirtualPathMap = std::unordered_map<
    std::string_view,
    CallbackType
  >;

  /**
   * @brief Default constructor, creates an empty object.
   * 
   */
  VirtualPathResolver() = default;

  /**
   * @brief Registers functor under the key which should invoke it
   * 
   * @param vPath Unique command key
   * @param callback Functor that should corespond to the key
   */
  void registerCallback(std::string_view vPath, CallbackType&& callback);

  /**
   * @brief Returns enviromental wrapper for running the command
   * 
   * @param target Command to be invoked
   * @return Query object that will be used for invokation of chosen target
   */
  Query newQuery(std::string_view target);

  /**
   * @brief Checks wheter there's a command registered under the target
   * 
   * @param target Comman to be checked
   * @return true if such command is registered
   * @return false otherwise
   */
  bool supportedOperation(std::string_view target);

private:

  VirtualPathMap pathMap_;

};



}