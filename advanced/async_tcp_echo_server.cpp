/*
 * async_tcp_echo_server.cpp
 *
 *  Created on: 2019Äê12ÔÂ17ÈÕ
 *      Author: poplarli
 */

#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class session : public std::enable_shared_from_this<session>{

public:
	session(tcp::socket socket) : socket_(std::move(socket)){
	}

	void start(){
		do_read();
	}

private:
	 void do_read(){
		 auto self = shared_from_this();
		 socket_.async_read_some(boost::asio::buffer(data_), [this, self](const boost::system::error_code& ec, std::size_t sz){
			 if(!ec){
				 do_write(sz);
			 }
		 });
	 }

	 void do_write(std::size_t sz){
		 auto self = shared_from_this();
		 boost::asio::async_write(socket_, boost::asio::buffer(data_, sz), [this, self](const boost::system::error_code& ec, std::size_t sz){
			 if(!ec){
				 do_read();
			 }
		 });
	 }

private:
	tcp::socket socket_;
	enum { max_length = 1024 };
	char data_[max_length];
};

class server{

public:
	server(boost::asio::io_context& io_context, short port) : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)){
		do_accept();
	}
private:
	void do_accept(){

		acceptor_.async_accept([this](const boost::system::error_code& ec, tcp::socket socket){
			if(!ec){
				std::make_shared<session>(std::move(socket))->start();
			}
			do_accept();
		});
	}

private:
	tcp::acceptor acceptor_;
};

int main(int argc, char** argv){

	if(argc != 2){
		std::cerr << "Usage: async_tcp_echo_server <port>\n";
		return 1;
	}

	boost::asio::io_context io_context;
	server s(io_context, atoi(argv[1]));
	io_context.run();

	return 0;
}


