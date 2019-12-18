/*
 * server.cpp
 *
 *  Created on: 2019Äê12ÔÂ18ÈÕ
 *      Author: poplarli
 */

#include "server.hpp"
#include <signal.h>
#include <utility>

namespace http {
namespace server {

		using boost::asio::ip::tcp;

		server::server(const std::string& address, const std::string& port,
			      const std::string& doc_root) :
			io_context_(1),
			signals_(io_context_),
			acceptor_(io_context_),
			connection_manager_(),
			request_handler_(doc_root){
				signals_.add(SIGINT);
				signals_.add(SIGTERM);
				#if defined(SIGQUIT)
				signals_.add(SIGQUIT);
				#endif // defined(SIGQUIT

				do_await_stop();
				boost::asio::ip::tcp::resolver resolver(io_context_);
				boost::asio::ip::tcp::endpoint endpoint =
				*resolver.resolve(address, port).begin();
				acceptor_.open(endpoint.protocol());
				acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
				acceptor_.bind(endpoint);
				acceptor_.listen();

				do_accept();
			}

		void server::do_accept(){

			acceptor_.async_accept([this](const boost::system::error_code& ec, tcp::socket&& socket){

		        if (!acceptor_.is_open())
		        {
		          return;
		        }

		        if(!ec){
		        	connection_manager_.start(std::make_shared<connection>(std::move(socket), connection_manager_, request_handler_));
		        }

		        do_accept();
			});

		}

		void server::do_await_stop(){
			signals_.async_wait([this](boost::system::error_code /*ec*/, int /*signo*/){
				acceptor_.close();
				connection_manager_.stop_all();
			});
		}

		void server::run(){
			io_context_.run();
		}
}
}


