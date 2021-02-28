#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

#include <iostream>

using tcp = boost::asio::ip::tcp;

void Log(boost::system::error_code ec) {
	std::cerr << (ec ? "Error: " : "OK")
	  	  << (ec ? ec.message() : "")
		  << std::endl;
}

int main() {
	boost::asio::io_context ioc{};

	tcp::socket socket{ioc};

	boost::system::error_code ec {};

	auto address {boost::asio::ip::address::from_string("1.1.1.1")};
	tcp::endpoint endpoint{address, 80};
	socket.connect(endpoint, ec);
	if (ec) {
		std::cerr << "Error: " << ec.message() << std::endl;
		return -1;
	} else {
		std::cout << "OK" << std::endl;
		return 0;
	}
}
