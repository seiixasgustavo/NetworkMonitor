#include <NetworkMonitor/websocket-client.h>

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/system/error_code.hpp>

#include <chrono>
#include <functional>
#include <iomanip>
#include <iostream>
#include <string>

using NetworkMonitor::WebSocketClient;

using tcp = boost::asio::ip::tcp;
namespace websocket = boost::beast::websocket;

// Static Methods

static void Log(const std::string &where, boost::system::error_code ec) {
  std::cerr << "[" << std::setw(20) << where << "] " << (ec ? "Error: " : "OK")
            << (ec ? ec.message() : "") << std::endl;
}

// Public Methods

WebSocketClient::WebSocketClient(const std::string &url,
                                 const std::string &port,
                                 boost::asio::io_context &ioc)
    : url_{url}, port_{port}, resolver_{boost::asio::make_strand(ioc)},
      ws_{boost::asio::make_strand(ioc)} {}

WebSocketClient::~WebSocketClient() = default;

void WebSocketClient::Connect(
    std::function<void(boost::system::error_code)> onConnect,
    std::function<void(boost::system::error_code, std::string &&)> onMessage,
    std::function<void(boost::system::error_code)> onDisconnect) {

  onConnect_ = onConnect;
  onMessage_ = onMessage;
  onDisconnect_ = onDisconnect;

  resolver_.async_resolve(
      url_, port_, [this](auto ec, auto endpoint) { OnResolve(ec, endpoint); });
}

void WebSocketClient::Send(
    const std::string &message,
    std::function<void(boost::system::error_code)> onSend) {

  boost::asio::const_buffer wBuffer{message.c_str(), message.size()};

  ws_.async_write(wBuffer,
                  [this, onSend](auto ec, auto nBytesWritten) { onSend(ec); });
}

void WebSocketClient::Close(
    std::function<void(boost::system::error_code)> onClose) {
  ws_.async_close(websocket::close_code::none, [this, onClose](auto ec) {
    if (onClose) {
      onClose(ec);
    }
  });
}

// Private Methods

void WebSocketClient::OnResolve(const boost::system::error_code &ec,
                                tcp::resolver::iterator endpoint) {
  if (ec) {
    Log("WebSocketClient.OnResolve", ec);
    if (onConnect_) {
      onConnect_(ec);
    }
    return;
  }

  ws_.next_layer().expires_after(std::chrono::seconds(5));

  ws_.next_layer().async_connect(*endpoint, [this](auto ec) { OnConnect(ec); });
}

void WebSocketClient::OnConnect(const boost::system::error_code &ec) {

  if (ec) {
    Log("WebSocketClient.OnConnect", ec);
    if (onConnect_) {
      onConnect_(ec);
    }
    return;
  }

  ws_.next_layer().expires_never();
  ws_.set_option(websocket::stream_base::timeout::suggested(
      boost::beast::role_type::client));

  ws_.async_handshake(url_, "/", [this](auto ec) { OnHandshake(ec); });
}

void WebSocketClient::OnHandshake(const boost::system::error_code &ec) {

  if (ec) {
    Log("WebSocketClient.OnHandshake", ec);
    if (onConnect_) {
      onConnect_(ec);
    }
    return;
  }

  ws_.text(true);

  ListenToIncomingMessage(ec);

  if (onConnect_) {
    onConnect_(ec);
  }
}

void WebSocketClient::ListenToIncomingMessage(
    const boost::system::error_code &ec) {
  if (ec == boost::asio::error::operation_aborted) {
    if (onDisconnect_) {
      onDisconnect_(ec);
    }
    return;
  }

  ws_.async_read(rBuffer_, [this](auto ec, auto nBytes) {
    OnRead(ec, nBytes);
    ListenToIncomingMessage(ec);
  });
}

void WebSocketClient::OnRead(const boost::system::error_code &ec,
                             size_t nBytes) {

  if (ec) {
    return;
  }

  std::string message{boost::beast::buffers_to_string(rBuffer_.data())};
  rBuffer_.consume(nBytes);
  onMessage_(ec, std::move(message));
}
