#include "Query.hpp"



namespace logic::vpath{


void Query::run() {
  query_(*this);
}


std::string_view Query::getTarget() const noexcept{
  return target_;
}


void Query::setTarget(std::string_view target) noexcept{
  target_ = target;
}


std::string_view Query::getData() const noexcept{
  return data_;
}


void Query::setData(std::string_view data) noexcept{
  data_ = data;
}


Query::ParameterMap& Query::getParameterMap() noexcept{
  return parameterMap_;
}


void Query::setParameterMap(Query::ParameterMap&& paramaterMap) noexcept{
  parameterMap_ = std::move(paramaterMap);
}


void Query::setErrorCallback(Query::ErrorCallback&& callback) noexcept{
  returnError_ = std::move(callback);
}


void Query::returnError(std::string_view message) const noexcept{
  returnError_(message);
}


void Query::setFileCallback(Query::FileCallback&& callback) noexcept{
  returnFile_ = std::move(callback);
}


void Query::returnFile(std::filesystem::path path) const noexcept{
  returnFile_(path);
}


void Query::setDataCallback(Query::DataCallback&& callback) noexcept{
  returnData_ = std::move(callback);
}


void Query::returnData(std::string&& data) const noexcept{
  returnData_(std::move(data));
}


void Query::setQueryProgram(QueryProgram callback) noexcept{
  query_ = callback;
}




}