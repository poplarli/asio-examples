/*
 * chat_client.cpp
 *
 *  Created on: 2019Äê12ÔÂ17ÈÕ
 *      Author: poplarli
 */

#ifndef ADVANCED_CHAT_CLIENT_CPP_
#define ADVANCED_CHAT_CLIENT_CPP_

#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include "chat_message.hpp"

using boost::asio::ip::tcp;

using chat_message_queue = std::deque<chat_message>;

class chat_client{

public:
	chat_client(boost::asio::io_context& ioc,
			const tcp::resolver::results_type& endpoints): ioc_(ioc), sock_(ioc_){
		do_connect(endpoints);
	}

	void do_connect(const tcp::resolver::results_type& endpoints){
		boost::asio::async_connect(sock_, endpoints, [this](const boost::system::error_code& ec, const tcp::endpoint&){
			if(!ec){
				do_read_header();
			}
		});
	}

	void do_read_header(){
		boost::asio::async_read(sock_, boost::asio::buffer(read_msg_.data(), chat_message::header_length),
				[this](const boost::system::error_code& ec, std::size_t sz){
			if(!ec  && read_msg_.decode_header()){
				std::cout << "ready to read body length = "  << read_msg_.body_length() << std::endl;
				do_read_body();
			}else{
				sock_.close();
			}
		});
	}

	void do_read_body(){
		boost::asio::async_read(sock_, boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
				[this](const boost::system::error_code& ec, std::size_t sz){
			if(!ec){
				std::cout.write(read_msg_.body(), read_msg_.body_length());
				std::cout << "\n";
				do_read_header();
			}else{
				sock_.close();
			}
		});
	}

	void write(const chat_message& msg){
		boost::asio::post(ioc_, [this, msg](){
			bool write_in_progress = !write_msgs_.empty();
			write_msgs_.push_back(msg);
			if(!write_in_progress){
				do_write();
			}
		});
	}

	void do_write(){
		boost::asio::async_write(sock_, boost::asio::buffer(write_msgs_.front().data(), write_msgs_.front().length()),
				[this](const boost::system::error_code& ec, std::size_t sz){
			//std::cout.write(write_msgs_.front().data(), write_msgs_.front().length());
			if(!ec){
				write_msgs_.pop_front();
				if(!write_msgs_.empty()){
					do_write();
				}
			}else{
				sock_.close();
			}
		});
	}

	void close(){
		boost::asio::post(ioc_, [this]{
			sock_,close();
		});
	}

private:
	boost::asio::io_context& ioc_;
	tcp::socket sock_;
	chat_message read_msg_;
	chat_message_queue write_msgs_;

};


int main(int argc, char** argv){
	try{

		if(argc != 3){
			std::cerr << "Usage: chat_client <host> <port> \n";
			return 1;
		}

		boost::asio::io_context ioc;

		tcp::resolver resolver(ioc);
		auto ep = resolver.resolve(argv[1], argv[2]);
		chat_client c(ioc, ep);

		std::thread t([&ioc]{ioc.run();});

		char line[chat_message::max_body_length + 1];
		while(std::cin.getline(line, chat_message::max_body_length + 1)){
			chat_message msg;
			msg.body_length(std::strlen(line));
			std::memcpy(msg.body(), line, msg.body_length());
			msg.encode_header();
			c.write(msg);
		}

		c.close();
		t.join();

	}catch (std::exception& e) {

	}
	return 0;
}


#endif /* ADVANCED_CHAT_CLIENT_CPP_ */
