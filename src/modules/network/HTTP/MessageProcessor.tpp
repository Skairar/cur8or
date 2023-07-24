#pragma once
#include "MessageProcessor.hpp"

#include "ResponseTemplate.hpp"
#include "ResponseFinalizer.hpp"
#include "logic/PathResolver/Query.hpp"


namespace network::http {


template <class PathResolver>
MessageProcessor<PathResolver>::MessageProcessor(
  std::shared_ptr<PathResolver> pathResolver
) : pathResolver_{pathResolver}
{
  //empty
}


template <class PathResolver>
void MessageProcessor<PathResolver>::operator()(
  MessageProcessor<PathResolver>::RequestType&& request,
  MessageProcessor<PathResolver>::ResponseCallback&& callback
) {

  using namespace boost::beast::http;

  if (false == pathResolver_->supportedOperation(request.target())){
    callback(ResponseTemplate{std::move(request)}.errorResponse(
      status::bad_request,
      "Illegal request target"
    ));
    return;
  }

  /*
    Should work for now, but I plan on expanding on it to support different
    methods depending on what's the target
  */
  switch (request.method()) {
    case verb::get:
    case verb::head:
    case verb::post:
      break;
    default:
      callback(ResponseTemplate{std::move(request)}.errorResponse(
        status::bad_request,
        "Unknown HTTP method"
      ));
      return;
  }

  std::shared_ptr<ResponseFinalizer> finalizer =
    std::make_shared<ResponseFinalizer>(
      std::move(request),
      std::move(callback)
    );

  logic::vpath::Query query = pathResolver_->newQuery(
    finalizer->getRequest().target()
  );

  switch (finalizer->getRequest().method()) {
    case verb::post:
      query.setData(finalizer->getRequest().body());
      break;
    default:
      break;
  }

  query.setErrorCallback(std::bind_front(
    &ResponseFinalizer::errorResponseCallback,
    finalizer
  ));
  query.setFileCallback(std::bind_front(
    &ResponseFinalizer::fileResponseCallback,
    finalizer
  ));
  query.setDataCallback(std::bind_front(
    &ResponseFinalizer::dataResponseCallback,
    finalizer
  ));

  query.run();

}

}