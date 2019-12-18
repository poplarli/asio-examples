/*
 * udpdaytimesrv.cpp
 *
 *  Created on: 2019Äê12ÔÂ16ÈÕ
 *      Author: poplarli
 */


#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/array.hpp>

using boost::asio::ip::udp;

std::string make_daytime_str(){
	time_t now = time(0);
	return ctime(&now);
}

int main(){

	boost::asio::io_context ioc;
	udp::socket sock(ioc, udp::endpoint(udp::v4(), 13));
	for(;;){
		boost::array<char, 1> recv_buf;
		udp::endpoint remote_endpoint;
		boost::system::error_code err;
		sock.receive_from(boost::asio::buffer(recv_buf), remote_endpoint);
		std::string message = make_daytime_str();
		boost::system::error_code ignore_err;
		sock.send_to(boost::asio::buffer(message), remote_endpoint, 0, ignore_err);
	}

	return 0;
}
