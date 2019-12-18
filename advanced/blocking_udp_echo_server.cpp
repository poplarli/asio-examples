/*
 * blocking_udp_echo_server.cpp
 *
 *  Created on: 2019Äê12ÔÂ17ÈÕ
 *      Author: poplarli
 */
#include <cstdlib>
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

enum { max_length = 1024 };

void server(boost::asio::io_context& ioc, int port){

	udp::socket sock(ioc, udp::endpoint(udp::v4(), port));

	for(;;){

		char buf[max_length];
		udp::endpoint ep;
		std::size_t len = sock.receive_from(boost::asio::buffer(buf, max_length), ep);

		sock.send_to(boost::asio::buffer(buf, len), ep);
	}

}

int main(int argc, char* argv[]){

    if (argc != 2)
    {
      std::cerr << "Usage: blocking_udp_echo_server <port>\n";
      return 1;
    }

    boost::asio::io_context ioc;
    server(ioc, atoi(argv[1]));

	return 0;
}
