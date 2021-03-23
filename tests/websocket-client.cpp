#include <NetworkMonitor/websocket-client.h>

#include <boost/asio.hpp>
#include <boost/test/unit_test.hpp>

#include <string>

using NetworkMonitor::WebSocketClient;

BOOST_AUTO_TEST_SUITE(NetworkMonitor);

BOOST_AUTO_TEST_CASE(class_WebSocketClient) {
  const std::string url{"echo.wss-websocket.net"};
  const std::string port{"80"};
  const std::string message{"Hello WebSocket"};

  boost::asio::io_context ioc{};

  WebSocketClient client{url, port, ioc};

  bool connected{false};
  bool messageSent{false};
  bool messageReceived{false};
  bool disconnected{false};
  std::string echo;

  auto onSend{[&messageSent](auto ec) { messageSent = !ec; }};

  auto onConnect{[&client, &connected, &onSend, &message](auto ec) {
    connected = !ec;
    if (!ec) {
      client.Send(message, onSend);
    }
  }};

  auto onClose{[&disconnected](auto ec) { disconnected = !ec; }};

  auto onReceive{[&client, &onClose, &messageReceived, &echo,
                  &message](auto ec, auto received) {
    messageReceived = !ec;
    echo = received;
    client.Close(onClose);
  }};

  client.Connect(onConnect, onReceive);
  ioc.run();

  BOOST_CHECK(connected);
  BOOST_CHECK(messageSent);
  BOOST_CHECK(messageReceived);
  BOOST_CHECK(disconnected);
  BOOST_CHECK_EQUAL(message, echo);
}

BOOST_AUTO_TEST_SUITE_END();
