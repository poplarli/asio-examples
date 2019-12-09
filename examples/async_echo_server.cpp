/*
 * async_echo_server.cpp
 *
 *  Created on: 2019Äê12ÔÂ6ÈÕ
 *      Author: poplarli
 */


#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>

using boost::asio::ip::tcp;

class Session : public boost::enable_shared_from_this<Session>{

public:
	Session(tcp::socket sock) : sock_(std::move(sock)){
	}

	void Start(){
		DoRead();
	}

	void DoRead(){
		auto self(shared_from_this());
		sock_.async_read_some(boost::asio::buffer(buf_), [self](boost::system::error_code err, std::size_t len){
			if(!err){
				self->DoWrite(len);
			}
		});
	}

	void DoWrite(std::size_t len){
		auto self(shared_from_this());
		boost::asio::async_write(sock_, boost::asio::buffer(buf_, len), [self](boost::system::error_code err, std::size_t len){
			if(!err){
				self->DoRead();
			}
		});
	}

private:
	tcp::socket sock_;
	std::array<char, 128> buf_;

};

class Server{

public:
	Server(boost::asio::io_context& ioc, std::uint16_t port) : acceptor_(ioc, tcp::endpoint(tcp::v4(), port)){
		DoAccept();
	}

private:
	void DoAccept(){
		acceptor_.async_accept([this](boost::system::error_code err, tcp::socket sock){
			if(!err){
				std::cout << sock.remote_endpoint().address().to_string() << ":" << sock.remote_endpoint().port() << std::endl;
				boost::shared_ptr<Session> sp(new Session(std::move(sock)));
				sp->Start();
			}
			this->DoAccept();
		});
	}

	tcp::acceptor acceptor_;

};

int main(int argc, char** argv){

	if(argc != 2){
		std::cerr<< "Usage " << argv[0] << " port" << std::endl;
		return 0;
	}

	boost::asio::io_context ioc;
	Server srv(ioc, atoi(argv[1]));
	ioc.run();

	return 0;
}
