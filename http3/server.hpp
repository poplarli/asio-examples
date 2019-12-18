/*
 * server.hpp
 *
 *  Created on: 2019Äê12ÔÂ18ÈÕ
 *      Author: poplarli
 */

#ifndef SERVER_HPP_
#define SERVER_HPP_

#include<boost/asio.hpp>
#include <string>
#include <vector>
#include "connection.hpp"
#include "request_handler.hpp"

namespace http{
	namespace server{
		class server{
		public:
			  server(const server&) = delete;
			  server& operator=(const server&) = delete;

			  explicit server(const std::string& address, const std::string& port,
				  const std::string& doc_root, std::size_t thread_pool_size);

			  void run();
		private:

			  void do_accept();
			  void do_await_stop();

			  boost::asio::io_context io_context_;
			  std::size_t thread_pool_size_;
			  boost::asio::signal_set signals_;
			  boost::asio::ip::tcp::acceptor acceptor_;
			  request_handler request_handler_;
		};
	}
}



#endif /* SERVER_HPP_ */
