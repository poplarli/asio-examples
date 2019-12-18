/*
 * process_per_connection.cpp
 *
 *  Created on: 2019年12月18日
 *      Author: poplarli
 */


#include <iostream>
#include <boost/asio.hpp>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <boost/asio/signal_set.hpp>

using boost::asio::ip::tcp;

class server{

public:

	server(boost::asio::io_context& ioc, int port) :
		io_context_(ioc),
		acceptor_(ioc, tcp::endpoint(tcp::v4(), port)),
		signal_(ioc, SIGCHLD),
		socket_(ioc)
	{
		std::cout << "server start " << std::endl;
		wait_for_signal();
		accept();
	}

	void wait_for_signal(){
		signal_.async_wait([this](const boost::system::error_code& ec, int sig){
			//acceptor_.is_open()证明是父进程
			if(acceptor_.is_open()){
				int status = 0;
				while(waitpid(-1, &status, WNOHANG) > 0){
				}

				//handler只会被调用一次，所以这里需要再次wait
				wait_for_signal();
			}
		});
	}

	void accept(){
		std::cout << "accept..." << std::endl;
		acceptor_.async_accept([this](const boost::system::error_code& ec, tcp::socket socket){

			std::cout << "new connect..." << std::endl;

			if(!ec){

				socket_ = std::move(socket);

				io_context_.notify_fork(boost::asio::io_context::fork_prepare);

				pid_t pid = fork();
				if(pid == 0){
					//子进程
					io_context_.notify_fork(boost::asio::io_context::fork_child);
					signal_.cancel();
					acceptor_.close();
					read();

				}else if(pid > 0){
					//父进程
					io_context_.notify_fork(boost::asio::io_context::fork_parent);
					socket_.close();
					accept();
				}
			}else{
					std::cerr << "Accept error: " << ec.message() << std::endl;
					accept();
			}
		});
	}


	void read(){
		socket_.async_read_some(boost::asio::buffer(data_), [this](const boost::system::error_code& ec, size_t len){

			if(!ec){
				write(len);
			}else{
				socket_.close();
			}

		});
	}

	void write(size_t len){

		boost::asio::async_write(socket_, boost::asio::buffer(data_, len), [this](const boost::system::error_code& ec, size_t len){
			if(!ec){
				read();
			}else{
				socket_.close();
			}
		});

	}

private:
	boost::asio::io_context& io_context_;
	tcp::acceptor acceptor_;
	boost::asio::signal_set signal_;
	tcp::socket socket_;
	std::array<char, 1024> data_;
};


int main(int argc, char** argv){

	try{

	    if (argc != 2)
	    {
	      std::cerr << "Usage: process_per_connection <port>\n";
	      return 1;
	    }

		boost::asio::io_context ioc;
		server s(ioc, atoi(argv[1]));
		ioc.run();


	}catch(std::exception& e){

		std::cerr << "Exception: " << e.what() << std::endl;

	}


	return 0;
}

