#pragma once

#include <functional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <filesystem>

#include "VirtualPathResolver.hpp"



namespace logic::vpath{



class VirtualPathResolver;



/**
 * @brief Object passed to VirtualPathResolver functors, serving as runtime
 *  environment for their execution and allowing them to pass on the result
 *  of their operations.
 * 
 * This class wraps all the data extracted from the request coming from the
 * higher layer of the program and also the callbacks used to return the
 * appriopriate response constructed from parsing it.
 * 
 * If we were to visualise functors as separate, closed programs, then Query
 * would be their runtime environment.
 * 
 * The functor is started by the function run() and has access to all the
 * necessary data and callbacks through the Query object. The functor itself
 * should pass it's data through one of the three return functions and then
 * return. Trying to invoke any of the return functions more than once or
 * invoking more than one type of retun function will result in the undefined
 * behaviour.
 * 
 */
class Query
{
  
  friend class VirtualPathResolver;

public:

  using ParameterMap = std::unordered_map<
    std::string_view,
    std::string_view
  >;

  using QueryProgram = std::function<void(Query&)>;
  using ErrorCallback = std::function<void(std::string_view)>;
  using FileCallback = std::function<void(std::filesystem::path)>;
  using DataCallback = std::function<void(std::string&&)>;

  /**
   * @brief Invokes the bound functor, passing itself as its argument
   * 
   */
  void run() noexcept;

  /**
   * @brief Get string representing the full command
   * 
   * @return std::string_view 
   */
  [[nodiscard]]
  std::string_view getTarget() const noexcept;

  /**
   * @brief Set the string representing the full command
   * 
   * Intended for specialized functors which use parts of the command itself
   * as their arguments and allows them to consume those and push the begin
   * of the string further.
   * 
   * @param newTarget New starting position for the command. As it's a view,
   *  it's mainly intended to be set to another position of the existing
   *  target string, as it doesn't own the memory it points to.
   * 
   */
  void setTarget(std::string_view newTarget) noexcept;

  /**
   * @brief Get the raw data passed to the Query
   * 
   * @return View of the raw data
   */
  [[nodiscard]]
  std::string_view getData() const noexcept;

  /**
   * @brief Set the Data object
   * 
   * Similar situation as with functions for target, mainly intended for
   * consuming parts of the data and moving the starting position forward
   * 
   * @param data 
   */
  void setData(std::string_view data) noexcept;

  /**
   * @brief Get the map of parameters passed to the Query
   * 
   * @return Reference to the map of parameters
   */
  [[nodiscard]]
  ParameterMap& getParameterMap() noexcept;

  /**
   * @brief Set the Parameter Map object
   * 
   * @param parameterMap Moved
   */
  void setParameterMap(ParameterMap&& parameterMap) noexcept;

  /**
   * @brief Set the Error Callback object
   * 
   * @param callback Moved
   */
  void setErrorCallback(ErrorCallback&& callback) noexcept;

  /**
   * @brief For returning errorous results
   * 
   * @param message Additional message to be attached with the error
   * 
   */
  void returnError(std::string_view message) const noexcept;

  /**
   * @brief Set the File Callback object
   * 
   * @param callback Moved
   */
  void setFileCallback(FileCallback&& callback) noexcept;

  /**
   * @brief For returning the path to the file that is supposed to be
   *  sent back with the response
   * 
   * @param path Location of the file that will be returned to the higher
   *  layer
   */
  void returnFile(std::filesystem::path path) const noexcept;

  /**
   * @brief Set the Data Callback object
   * 
   * @param callback Moved
   */
  void setDataCallback(DataCallback&& callback) noexcept;

  /**
   * @brief For returning serialized JSON data
   * 
   * @param data Serialized JSON string
   */
  void returnData(std::string&& data) const noexcept;

private:

  /**
   * @brief Privated constructor
   * 
   * Instances of this class should be created by the object that manages the
   * functors
   */
  Query() = default;

  /**
   * @brief Sets the Functor which this Query will invoke, passing itself as
   *  its argument
   * 
   * @param callback The functor for which the object of this class will serve
   *  as runtime environment
   */
  void setQueryProgram(QueryProgram callback) noexcept;

  std::string_view target_,
                   data_;
  
  ParameterMap parameterMap_;
  QueryProgram query_;
  ErrorCallback returnError_;
  FileCallback returnFile_;
  DataCallback returnData_;

};



}