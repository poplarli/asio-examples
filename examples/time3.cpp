/*
 * time3.cpp
 *
 *  Created on: 2019Äê12ÔÂ16ÈÕ
 *      Author: poplarli
 */


#include <iostream>
#include <boost/asio.hpp>
#include <functional>


void print(boost::system::error_code&, boost::asio::steady_timer* pt, size_t* pc){

	if(*pc >= 5){
		return;
	}

	std::cout << "cnt = " << *pc++ << std::endl;

	pt->async_wait(std::bind(&print, std::placeholders::_1, pt, pc));
}

int main(){

	boost::asio::io_context ioc;
	boost::asio::steady_timer t(ioc, boost::asio::chrono::seconds(1));

	size_t cnt = 0;
	t.async_wait(std::bind(&print, std::placeholders::_1, &t, &cnt));

	ioc.run();

	return 0;
}

