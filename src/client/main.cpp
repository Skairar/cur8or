#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <boost/config.hpp>
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>
#include <exception>

#include <unordered_map>
#include <string_view>
#include <filesystem>

#include "network/SocketListener.hpp"
#include "network/HTTP/SimpleSession.hpp"
#include "network/HTTP/MessageProcessor.hpp"
#include "logic/PathResolver/VirtualPathResolver.hpp"
#include "logic/PathResolver/Query.hpp"
#include "utilities/adaptors/ParamPathAdaptor.hpp"


namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>


int main(int argc, char* argv[])
{
    // Check command line arguments.
    if (argc != 5)
    {
        std::cerr <<
            "Usage: http-server-async <address> <port> <doc_root> <threads>\n" <<
            "Example:\n" <<
            "    http-server-async 0.0.0.0 8080 . 1\n";
        return EXIT_FAILURE;
    }
    auto const address = net::ip::make_address(argv[1]);
    auto const port = static_cast<unsigned short>(std::atoi(argv[2]));
    std::filesystem::path interfaceRoot{argv[3]};
    auto const threads = std::max<int>(1, std::atoi(argv[4]));

    // The io_context is required for all I/O
    net::io_context ioc{threads};

    std::shared_ptr<logic::vpath::VirtualPathResolver> vpath =
     std::make_shared<logic::vpath::VirtualPathResolver>();

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

    std::make_shared<network::SocketListener>(
        ioc,
        tcp::endpoint{address, port},
        [&](tcp::socket socket) {
          std::make_shared<network::http::SimpleSession>(
            std::move(socket),
            network::http::MessageProcessor{vpath}
            )->startSession();
        })->acceptUpcoming();

    std::vector<std::thread> v;
    v.reserve(threads - 1);
    for(auto i = threads - 1; i > 0; --i)
        v.emplace_back(
        [&ioc]
        {
            ioc.run();
        });
    ioc.run();

    return EXIT_SUCCESS;
}