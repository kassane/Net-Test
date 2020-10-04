#include "networking.hpp"
#include <iostream>
#include <stx/panic.h>

void stx::panic_handler(
    const std::string_view &info, const stx::ReportPayload &payload,
    [[maybe_unused]] const stx::SourceLocation &source_location) noexcept {
  std::cerr << "Error Occurred"
            << "\nInfo: " << info //
            << "\nPayload:" << payload.data() << std::endl;
}
//----------------------------------------------------------------------------

Networking::Networking(net::io_context &io,
                       [[maybe_unused]] net::error_code &ec)
    : mSocket(io), mEc(ec) {
  // TODO
}
//----------------------------------------------------------------------------

void Networking::host(const std::string &address, const unsigned port) {
  mAddr = address;
  mPort = port;
  net::ip::tcp::endpoint ep(net::ip::make_address(address, mEc), port);
  mSocket.connect(ep, mEc);
  if (!mEc)
    std::cout << "Connected!\n\nPort:[" << port
              << "]\n\n-------------------------\n";
  else
    std::cerr << "Error: [" << port << "] " << mEc.message() << std::endl;
  mSocket.close();
}

void Networking::request_view() {
  net::ip::tcp::iostream net_io(mAddr, "http");
  if (!net_io) {
    stx::panic(net_io.error().message());
  }

  net_io << "GET / HTTP/1.1\r\n";
  net_io << "Host: " << mAddr << "\r\n";
  net_io << "Accept: */*\r\n";
  net_io << "Connection: close\r\n\r\n";
  std::string header;
  while (std::getline(net_io, header) && header != "\r") {
    std::cout << header << std::endl << net_io.rdbuf() << std::endl;
  }
}

Networking::~Networking() { mSocket.close(); }
