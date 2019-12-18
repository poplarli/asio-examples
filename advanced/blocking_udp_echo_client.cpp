/*
 * blocking_udp_echo_client.cpp
 *
 *  Created on: 2019Äê12ÔÂ17ÈÕ
 *      Author: poplarli
 */


#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

enum { max_length = 1024 };

int main(int argc, char* argv[]){

	boost::asio::io_context ioc;

	udp::socket sock(ioc, udp::endpoint(udp::v4(), 0));

	udp::resolver resolver(ioc);
	udp::resolver::results_type eps = resolver.resolve(udp::v4(), argv[1], argv[2]);

	std::cout << "Enter message: ";
	char request[max_length];
	std::cin.getline(request, max_length);
	size_t request_length = std::strlen(request);

	sock.send_to(boost::asio::buffer(request, request_length), *eps.begin());

	char reply[max_length];
	udp::endpoint sender_endpoint;

	size_t reply_length =sock.receive_from(boost::asio::buffer(reply, max_length), sender_endpoint);

	std::cout << "Reply is: ";
	std::cout.write(reply, reply_length);
	std::cout << "\n";
	return 0;
}

