#ifndef NETWORKING_HPP
#define NETWORKING_HPP
#include <asio/error_code.hpp> // for error_code, asio
#include <asio/ip/tcp.hpp>     // for tcp, tcp::socket
#include <memory>              // for enable_shared_from_this
#include <string>              // for string
namespace asio {
class io_context;
}
#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif

namespace net = asio;

class Networking : public std::enable_shared_from_this<Networking> {
public:
  Networking(net::io_context &io, net::error_code &ec);
  void host(const std::string &address, const unsigned port);
  void request_view();
  ~Networking();

private:
  net::ip::tcp::socket mSocket;
  net::error_code mEc;
  std::string mAddr = "";
  unsigned mPort = 0;
};

#endif // NETWORKING_HPP
