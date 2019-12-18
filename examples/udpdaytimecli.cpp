/*
 * udpdaytimecli.cpp
 *
 *  Created on: 2019Äê12ÔÂ16ÈÕ
 *      Author: poplarli
 */

#include <iostream>
#include <boost/asio.hpp>
#include <boost/array.hpp>

using boost::asio::ip::udp;

int main(int argc, char** argv){

	if(argc != 2){
		return 0;
	}

	boost::asio::io_context ioc;
	udp::resolver reso(ioc);
	udp::endpoint receiver_endpoint = * reso.resolve(udp::v4(), argv[1], "daytime").begin();

	udp::socket sock(ioc);
	sock.open(udp::v4());
	boost::array<char , 1> send_buf = {{0}};
	sock.send_to(boost::asio::buffer(send_buf), receiver_endpoint);

	boost::array<char , 128> re_buf;
	udp::endpoint ep;
	size_t len = sock.receive_from(boost::asio::buffer(re_buf), ep);
	std::cout.write(re_buf.data(), len);

	return 0;
}


