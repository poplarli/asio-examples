/*
 * aync_daytimesrc.cpp
 *
 *  Created on: 2019年12月5日
 *      Author: poplarli
 */


#include <iostream>
#include <string>
#include <ctime>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>

using boost::asio::ip::tcp;

namespace{
	std::string make_daytime_str(){
		time_t now = time(0);
		return ctime(&now);
	}
};


class Tcp_connnection : public boost::enable_shared_from_this<Tcp_connnection>{

public:
	//shared_ptr封装，保证异步模式下对象的声明周期
	typedef boost::shared_ptr<Tcp_connnection> pointer;

	static pointer create(boost::asio::io_context& ioc){
		return pointer(new Tcp_connnection(ioc));
	}

	tcp::socket& socket(){
		return sock_;
	}

	void start(){
		msg_ = make_daytime_str();

		//使用shared_from_this防止Tcp_connnection提前析构
		boost::asio::async_write(sock_,
				boost::asio::buffer(msg_),
				boost::bind(&Tcp_connnection::handle_write, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
	}

private:
	Tcp_connnection(boost::asio::io_context& ioc) : sock_(ioc){
	}

	void handle_write(const boost::system::error_code& err, size_t bytes){
		std::cout << "write end: bytes = " << bytes << std::endl;
	}

	tcp::socket sock_;
	std::string msg_;
};


class Tcp_server{


public:
	//初始化acceptor_，然后调用start_accept
	Tcp_server(boost::asio::io_context& ioc) : acceptor_(ioc, tcp::endpoint(tcp::v4(), 13)){
		start_accept();
	}

private:

	//每次accept一个连接
	void start_accept(){
		Tcp_connnection::pointer new_conn = Tcp_connnection::create(acceptor_.get_executor().context());
		acceptor_.async_accept(new_conn->socket(),
				boost::bind(&Tcp_server::handle_accept, this, new_conn, boost::asio::placeholders::error));
	}

	void handle_accept(Tcp_connnection::pointer new_conn, const boost::system::error_code& err){
		if(!err){
			new_conn->start();
		}
		//再次调用start_accept
		start_accept();
	}

	tcp::acceptor acceptor_;
};


int main(int agrc, char ** argv){

	try{

		boost::asio::io_context ioc;
		Tcp_server server(ioc);
		ioc.run();

	}catch(std::exception& e){
		std::cerr << e.what() << std::endl;
	}

	return 0;
}


