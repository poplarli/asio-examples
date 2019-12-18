/*
 * async_daytimesrc.cpp
 *
 *  Created on: 2019Äê12ÔÂ16ÈÕ
 *      Author: poplarli
 */

#include <iostream>
#include <functional>
#include <memory>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

using boost::asio::ip::tcp;

std::string make_daytime_str(){
	time_t now = time(0);
	return ctime(&now);
}

class Connection : public std::enable_shared_from_this<Connection>{

public:
	using Pointer = std::shared_ptr<Connection>;

	Connection(boost::asio::io_context& ioc) : sock_(ioc){
	}

	static Pointer createConnection(boost::asio::io_context& ioc){
		return std::make_shared<Connection>(ioc);
	}

	tcp::socket& getSocket(){
		return sock_;
	}

	void start(){

		boost::asio::async_write(sock_, boost::asio::buffer(make_daytime_str()), [](const boost::system::error_code& err, size_t sz)
		{
			std::cout << sz << " bytes transferred~" << std::endl;
		});

	}


private:
	tcp::socket sock_;
};

class TcpServer{

public:
	TcpServer(boost::asio::io_context& ioc) : acceptor_(ioc, tcp::endpoint(tcp::v4(), 13)){
		start();
	}

	void start(){

		Connection::Pointer con = Connection::createConnection(acceptor_.get_executor().context());
		acceptor_.async_accept(con->getSocket(), [con, this](const boost::system::error_code& err) {

			if(!err){
				con->start();
			}

			start();
		});

	}


private:
	tcp::acceptor acceptor_;
};

int main(){

	boost::asio::io_context ioc;

	TcpServer server(ioc);

	ioc.run();

	return 0;

}


