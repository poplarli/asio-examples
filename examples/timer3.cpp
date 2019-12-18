/*
 * timer3.cpp
 *
 *  Created on: 2019Äê12ÔÂ16ÈÕ
 *      Author: poplarli
 */


#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <functional>


void print(const boost::system::error_code&, boost::asio::steady_timer* pt, size_t* pc){

	if(*pc >= 5){
		return;
	}

	std::cout << "cnt = " << *pc << std::endl;

	++(*pc);

	pt->expires_at(pt->expiry() + boost::asio::chrono::seconds(1));
	pt->async_wait(boost::bind(print, boost::asio::placeholders::error, pt, pc));
}

int main(){

	boost::asio::io_context ioc;
	boost::asio::steady_timer t(ioc, boost::asio::chrono::seconds(1));

	size_t cnt = 0;
	t.async_wait(boost::bind(print, boost::asio::placeholders::error, &t, &cnt));

	ioc.run();

	std::cout << "final cnt is " << cnt << std::endl;

	return 0;
}



