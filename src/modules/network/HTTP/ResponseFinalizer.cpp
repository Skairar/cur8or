#include "ResponseFinalizer.hpp"


namespace network::http{



ResponseFinalizer::ResponseFinalizer(
  ResponseFinalizer::RequestType&& request,
  ResponseFinalizer::ResponseCallback&& callback
) noexcept
  : request_{std::move(request)},
    callback_{std::move(callback)}
{
  //empty
}


ResponseFinalizer::RequestType& ResponseFinalizer::getRequest() noexcept{
  return request_;
}


void ResponseFinalizer::errorResponseCallback(
  std::string_view message
) noexcept{

  callback_(
    ResponseTemplate{std::move(request_)}.errorResponse(
      StatusType::bad_request,
      message
    )
  );

}


void ResponseFinalizer::fileResponseCallback(
  const ResponseFinalizer::PathType& path
) noexcept{

  callback_(
    ResponseTemplate{std::move(request_)}.fileResponse(path)
  );

}


void ResponseFinalizer::dataResponseCallback(
  std::string&& data
) noexcept{

  callback_(
    ResponseTemplate{
      std::move(request_)
    }.dataResponse(std::move(data))
  );

}


}//namespace network::http