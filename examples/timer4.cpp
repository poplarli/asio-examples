/*
 * timer4.cpp
 *
 *  Created on: 2019Äê12ÔÂ16ÈÕ
 *      Author: poplarli
 */

#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <memory>


class Printer{


public:
	Printer(boost::asio::io_context& ioc) : t(ioc, boost::asio::chrono::seconds(1)), cnt(0){
		t.async_wait(boost::bind(&Printer::print, this));
	}

	void print(){
		std::cout << "cnt is " << cnt << std::endl;

		++cnt;

		t.expires_at(t.expiry() + boost::asio::chrono::seconds(1));
		t.async_wait(boost::bind(&Printer::print, this));
	}

	~Printer(){
		std::cout << "final cnt is " << cnt << std::endl;
	}

private:
	boost::asio::steady_timer t;
	size_t cnt;
};

int main(){

	boost::asio::io_context ioc;
	std::unique_ptr<Printer> sp(std::make_unique<Printer>(ioc));
	ioc.run();
	return 0;
}


