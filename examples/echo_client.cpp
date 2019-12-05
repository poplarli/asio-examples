/*
 * echo_client.cpp
 *
 *  Created on: 2019Äê12ÔÂ5ÈÕ
 *      Author: poplarli
 */

#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main(int agrc, char** argv){


	boost::asio::io_context ioc;
	tcp::socket sock(ioc);

	auto ip = argv[1];
	auto port = argv[2];
	tcp::resolver resolver(ioc);
	auto endpoints = resolver.resolve(tcp::v4(), ip, argv[2]);
	boost::asio::connect(sock, endpoints);

	char req[128];
	auto len = 0;
	do{
		std::cout << "enter a msg" << std::endl;
		std::cin.getline(req, 128);
		len = std::strlen(req);

	}while(len == 0);

	boost::asio::write(sock, boost::asio::buffer(req, len));

	std::cout << "send " << len << " bytes" << std::endl;

	char reply[128];
	auto read_len = boost::asio::read(sock, boost::asio::buffer(reply, len));

	std::cout.write(reply, read_len);
	std::cout << std::endl;
	std::cout << "read " << read_len << " bytes" << std::endl;


	return 0;
}


