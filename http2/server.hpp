/*
 * server.hpp
 *
 *  Created on: 2019Äê12ÔÂ19ÈÕ
 *      Author: poplarli
 */

#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <boost/asio.hpp>
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "connection.hpp"
#include "io_context_pool.hpp"
#include "request_handler.hpp"

namespace http{
	namespace server{

		class server{
		public:
			server(const server&) = delete;
			server& operator=(const server&) = delete;
			server(const std::string& address, const std::string& port,
				      const std::string& doc_root, std::size_t io_context_pool_size);

			void run();

		private:
			void do_accept();
			void do_await_stop();


			io_context_pool pool_;
			boost::asio::signal_set signals_;
			boost::asio::ip::tcp::acceptor acceptor_;
			connection_ptr new_connection_;
			request_handler request_handler_;
		};
	}
}



#endif /* SERVER_HPP_ */
