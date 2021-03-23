#include <NetworkMonitor/websocket-client.h>

#include <boost/asio.hpp>

#include <iostream>
#include <string>

using NetworkMonitor::WebSocketClient;

int main() {
  const std::string url{"echo.wss-websocket.net"};
  const std::string port{"80"};
  const std::string message{"Hello WebSocket"};

  boost::asio::io_context ioc{};

  WebSocketClient client{url, port, ioc};

  bool connected{false};
  bool messageSent{false};
  bool messageReceived{false};
  bool messageMatches{false};
  bool disconnected{false};

  auto onSend{[&messageSent](auto ec) { messageSent = !ec; }};

  auto onConnect{[&client, &connected, &onSend, &message](auto ec) {
    connected = !ec;
    if (!ec) {
      client.Send(message, onSend);
    }
  }};

  auto onClose{[&disconnected](auto ec) { disconnected = !ec; }};

  auto onReceive{[&client, &onClose, &messageReceived, &messageMatches,
                  &message](auto ec, auto received) {
    messageReceived = !ec;
    messageMatches = message == received;
    client.Close(onClose);
  }};

  client.Connect(onConnect, onReceive);
  ioc.run();

  bool ok{connected && messageSent && messageReceived && messageMatches &&
          disconnected};
  if (ok) {
    std::cout << "OK" << std::endl;
    return 0;
  } else {
    std::cerr << "Test failed" << std::endl;
    return 1;
  }
}
