/*
 * blocking_tcp_echo_server.cpp
 *
 *  Created on: 2019Äê12ÔÂ17ÈÕ
 *      Author: poplarli
 */

#include <cstdlib>
#include <iostream>
#include <thread>
#include <utility>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
enum {max_length = 1024};


void session(tcp::socket sock){

	for(;;){
		char buf[max_length];
		 boost::system::error_code ec;
		size_t len = sock.read_some(boost::asio::buffer(buf), ec);
		if(!ec){
			boost::asio::write(sock, boost::asio::buffer(buf, len));
		}else{
			sock.close();
		}
	}

}


int main(){

	boost::asio::io_context ioc;

	tcp::acceptor acceptor(ioc, tcp::endpoint(tcp::v4(), 8080));

	for(;;){
		std::thread(session, acceptor.accept()).detach();
	}

	return 0;
}

