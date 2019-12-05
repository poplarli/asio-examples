/*
 * echo_server.cpp
 *
 *  Created on: 2019年12月5日
 *      Author: poplarli
 */

#include <iostream>
#include <boost/asio.hpp>
#include <boost/array.hpp>

using boost::asio::ip::tcp;

void session(tcp::socket sock){

	try{
		while(1){
			boost::array<char, 128> buf;
			boost::system::error_code err;
			std::size_t len = sock.read_some(boost::asio::buffer(buf), err);
			if(err == boost::asio::error::eof){
				break;
			}else if(err){
				throw boost::system::system_error(err);
			}
			boost::asio::write(sock, boost::asio::buffer(buf, len));
		}

	}catch(std::exception& e){
		std::cerr << "Exception: " <<  e.what() << std::endl;
	}

}

int main(int argc, char** argv){


	if(argc != 2){
	    std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
	    return 1;
	}

	auto port = std::atoi(argv[1]);

	//tcp::acceptor是一个io对象，用来接收tcp连接的，端口由tcp::endpoint指定
	boost::asio::io_context ioc;
	tcp::acceptor ator(ioc, tcp::endpoint(tcp::v4(), port));

	try{
		//迭代处理。 accept、read_some 和 write都是阻塞的。
		while(1){
			session(ator.accept());
		}
	}catch(std::exception& e){
		std::cerr << "Exception: " <<  e.what() << std::endl;
	}
	return 0;
}
