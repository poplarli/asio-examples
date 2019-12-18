/*
 * timer2.cpp
 *
 *  Created on: 2019Äê12ÔÂ16ÈÕ
 *      Author: poplarli
 */

#include <iostream>
#include <boost/asio.hpp>

void print(const boost::system::error_code&){
	std::cout << "hello world" << std::endl;
}

int main(){

	boost::asio::io_context ioc;
	boost::asio::steady_timer t(ioc, boost::asio::chrono::seconds(3));

	t.async_wait(&print);

	ioc.run();

	return 0;
}


