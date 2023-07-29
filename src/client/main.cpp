#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <boost/config.hpp>
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <thread>
#include <vector>
#include <filesystem>

#include <boost/leaf/handle_errors.hpp>

#include <boost/json.hpp>

#include "network/SocketListener.hpp"
#include "network/HTTP/SimpleSession.hpp"
#include "network/HTTP/MessageProcessor.hpp"
#include "logic/PathResolver/VirtualPathResolver.hpp"
#include "logic/PathResolver/Query.hpp"
#include "utilities/adaptors/ParamPathAdaptor.hpp"
#include "utilities/adaptors/KeyValueAdaptor.hpp"
#include "database/embedded/SQLite3/Instance.hpp"
#include "database/embedded/SQLite3/Statement.hpp"
#include "database/embedded/SQLite3/serialization/JSON.hpp"


namespace asio = boost::asio;   
using tcp = boost::asio::ip::tcp;       


int main(int argc, char* argv[])
{
  using namespace database::embedded::SQLite3;
  
  auto const address = asio::ip::make_address("0.0.0.0");
  auto const port = 8080;
  std::filesystem::path interfaceRoot{"./root"};
  auto const threads = 4;

  asio::io_context context{threads};

  /*

  */
  std::shared_ptr<Instance> database = boost::leaf::try_handle_all(
    []() -> boost::leaf::result<std::shared_ptr<Instance>> {
      BOOST_LEAF_AUTO(database, Instance::newInstance(
        std::filesystem::path{"./cur8-test.db"},
        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_FULLMUTEX
        ));
      return database;
    },
    [](int errorCode, std::string errorMsg) -> std::shared_ptr<Instance> {
      std::cout << "Database error " << errorCode << ": "
                << errorMsg << "\nTerminating." << std::endl;
      std::exit(EXIT_FAILURE);
      //won't reach
      return std::shared_ptr<Instance>{nullptr};
    },
    []() -> std::shared_ptr<Instance> {
      //won't reach, but again, necessary
      std::exit(EXIT_FAILURE);
      return std::shared_ptr<Instance>{nullptr};
    }
  );

  std::shared_ptr<logic::vpath::VirtualPathResolver> vpath =
    std::make_shared<logic::vpath::VirtualPathResolver>();

  vpath->registerCallback(
    "",
    [interfaceRoot](logic::vpath::Query& query) mutable{
      interfaceRoot /= "Query.html";
      query.returnFile(interfaceRoot);
    }
  );

  vpath->registerCallback(
    "file",
    [interfaceRoot](logic::vpath::Query& query) mutable{
      auto pos = query.getTarget().find('/', 1);
      if (pos == std::string_view::npos) {
        query.returnError("No file path");
        return;
      }
      interfaceRoot.concat(query.getTarget().substr(pos));
      if (std::filesystem::is_directory(interfaceRoot)){
        interfaceRoot /= "index.html";
      }
      query.returnFile(interfaceRoot);
    }
  );

  vpath->registerCallback(
    "query",
    [&database](logic::vpath::Query& query) mutable{
      using namespace utilities::adaptors;
      using namespace boost;
      KeyValueAdaptor::MapType postValues = query.getData() |  KeyValueAdaptor{};
      std::string_view sql = postValues.find("sql")->second;
      leaf::result<Table> table = leaf::try_handle_some(
        [&database, sql]() -> leaf::result<Table>{
          BOOST_LEAF_AUTO(statement, database->prepareStatement(sql));
          BOOST_LEAF_AUTO(table, statement->evaluate());
          return table;
        },
        [query](int errorCode, std::string errorMsg)
          -> leaf::result<Table> {
          std::stringstream errorMessage;
          errorMessage << "SQL failure, code: " << errorCode
                       << " - " << errorMsg << std::endl;
          //Thinking about it now, it's somewhat messy way to do that,
          //it works only because the entire message sending is immediately
          //invoked and processed before we quite this scope, but in the future
          //the lifetime of the message might not be as certain.
          //Perhaps giving Query it's own streams for output might be worth
          //consideration
          query.returnError(errorMessage.str());
          return leaf::new_error("empty");
        }
      );
      //Yea, leaf is great and all, but ideally I'd want the invokation of
      //Query callback to finish the moment one of its return functions is
      //invoked without additionaly having to make sure each time that
      //shit wont go South, will likely need to wrap it into additional
      //abstraction layer for runtime.
      if (table.has_error()) {
        return;
      }

      query.returnData(json::serialize(json::value_from(table.value())));
    }
  );

  std::make_shared<network::SocketListener>(
    context,
    tcp::endpoint{address, port},
    [&](tcp::socket socket) {
      std::make_shared<network::http::SimpleSession>(
        std::move(socket),
        network::http::MessageProcessor{vpath}
      )->startSession();
    })->acceptUpcoming();

  std::vector<std::thread> threadVector;
  threadVector.reserve(threads - 1);
  for(int i = threads - 1; i > 0; --i)
    threadVector.emplace_back(
      [&context]
      {
        context.run();
      });
  context.run();

  return EXIT_SUCCESS;

}