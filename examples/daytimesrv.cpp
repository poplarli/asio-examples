/*
 * daytimesrv.cpp
 *
 *  Created on: 2019年12月5日
 *      Author: poplarli
 */



#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

std::string make_dattime_str(){
	time_t now = time(0);
	return ctime(&now);
}

int main(int argc, char** argv){

	try{

		boost::asio::io_context io_context;
		tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 13));

		for(;;){
			//同步IO
			//先accept，再write
			tcp::socket socket(io_context);
			acceptor.accept(socket);

			std::string msg = make_dattime_str();

			boost::system::error_code err;
			boost::asio::write(socket, boost::asio::buffer(msg), err);
		}

	}catch(std::exception& e){
		std::cerr << e.what() << std::endl;
	}

	return 0;
}



