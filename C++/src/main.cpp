#include "networking.hpp"
#include <cxxopts.hpp>
#include <iostream>    // for operator<<
#include <numeric>     // iota
#include <string_view> // for string_...
#include <vector>      // for vector

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {

  cxxopts::Options options("NetSafer", "Network Manager tool");
  net::io_context io;
  net::error_code ec;

  options.add_options()(
      "a, address", "Host address",
      cxxopts::value<std::string>()->default_value("127.0.0.1"))(
      "p, port", "Port param",
      cxxopts::value<int>()->default_value("80"))("h, help", "Print usage")(
      "s, scan", "Port Scan",
      cxxopts::value<std::string>()->default_value("127.0.0.1"));
  options.custom_help("[OPTION...] destination");

  try {
    auto result = options.parse(argc, argv);

    if (result.count("help") || argc < 2) {
      std::cout << options.help() << "\nExamples:\n"
                << "Request: " << argv[0] << " -a 192.168.0.1 -p 80"
                << std::endl;
      std::cout << "Port Scan: " << argv[0] << " -s 192.168.0.1" << std::endl;
      return EXIT_SUCCESS;
    }
    if (result.count("address") || result.count("port")) {
      auto rede = std::make_unique<Networking>(io, ec);
      rede->host(result["address"].as<std::string>(), result["port"].as<int>());
      rede->request_view();
    }
    if (result.count("scan")) {
      auto rede = std::make_unique<Networking>(io, ec);

      std::vector<unsigned> ports(1000);
      std::iota(ports.begin(), ports.end(), 1);
      for (auto &port : ports) {
        rede->host(result["scan"].as<std::string>(), port);
      }
    }
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
