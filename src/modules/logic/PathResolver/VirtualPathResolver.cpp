#include "VirtualPathResolver.hpp"

#include "utilities/adaptors/ParamPathAdaptor.hpp"



namespace logic::vpath {


void VirtualPathResolver::registerCallback(
  std::string_view vpath,
  VirtualPathResolver::CallbackType&& callback
) {
  pathMap_.emplace(vpath, std::move(callback));
}


Query VirtualPathResolver::newQuery(std::string_view target) {
  using namespace utilities::adaptors;
  Query query{};
  query.setTarget(target);
  query.setQueryProgram(
    pathMap_.at((target | ParamPathAdaptor{}).front().first)
  );
  return query;
}


//temporary
bool VirtualPathResolver::supportedOperation(std::string_view vpath) {
  using namespace utilities::adaptors;
  return (!(
    vpath.empty() ||
    vpath[0] != '/' ||
    vpath.find("..") != vpath.npos ||
    !(pathMap_.contains((vpath | ParamPathAdaptor{}).front().first))
  ));
}


}