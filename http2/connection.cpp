/*
 * connection.cpp
 *
 *  Created on: 2019��12��19��
 *      Author: poplarli
 */


#include "connection.hpp"
#include <vector>
#include "request_handler.hpp"

namespace http{
	namespace server{

		connection::connection(boost::asio::io_context& io_context,  request_handler& handler)
		  : socket_(io_context),
			request_handler_(handler){
		}

		boost::asio::ip::tcp::socket& connection::socket()
		{
		  return socket_;
		}

		void connection::start(){
			do_read();
		}

		void connection::do_read(){
			auto self(shared_from_this());
				socket_.async_read_some(boost::asio::buffer(buffer_), [this, self](const boost::system::error_code& ec, size_t bytes_transferred){
					if(!ec){
					  request_parser::result_type result;
					  std::tie(result, std::ignore) = request_parser_.parse(
						  request_, buffer_.data(), buffer_.data() + bytes_transferred);

					  if (result == request_parser::good)
					  {
						request_handler_.handle_request(request_, reply_);
						do_write();
					  }
					  else if (result == request_parser::bad)
					  {
						reply_ = reply::stock_reply(reply::bad_request);
						do_write();
					  }
					  else
					  {
						do_read();
					  }
					}
				});
		}

		void connection::do_write(){
			auto self(shared_from_this());
			boost::asio::async_write(socket_, reply_.to_buffers(), [this, self](boost::system::error_code ec, std::size_t){
				if (!ec)
				{
				  // Initiate graceful connection closure.
				  boost::system::error_code ignored_ec;
				  socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both,
					ignored_ec);
				}
			});
		}

	}
}


