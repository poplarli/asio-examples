/*
 * server.cpp
 *
 *  Created on: 2019Äê12ÔÂ18ÈÕ
 *      Author: poplarli
 */

#include "server.hpp"
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>

namespace http {
namespace server {

using boost::asio::ip::tcp;

		server::server(const std::string& address, const std::string& port,
			      const std::string& doc_root, std::size_t thread_pool_size) :
			io_context_(1),
			thread_pool_size_(thread_pool_size),
			signals_(io_context_),
			acceptor_(io_context_),
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


		void server::do_await_stop(){
			signals_.async_wait([this](boost::system::error_code /*ec*/, int /*signo*/){
				io_context_.stop();
			});
		}

		void server::do_accept(){

			acceptor_.async_accept([this](const boost::system::error_code& ec, tcp::socket socket){

		        if (!acceptor_.is_open())
		        {
		          return;
		        }

		        if(!ec){
		        	std::make_shared<connection>(io_context_, std::move(socket), request_handler_)->start();
		        }

		        do_accept();
			});

		}


		void server::run(){

				std::vector<std::shared_ptr<boost::thread>> threads;
				for(std::size_t i = 0; i < thread_pool_size_; ++i){
					std::shared_ptr<boost::thread> thread(new boost::thread(boost::bind(&boost::asio::io_context::run, &io_context_)));
					threads.push_back(thread);
				}

			  // Wait for all threads in the pool to exit.
			  for (std::size_t i = 0; i < threads.size(); ++i)
			    threads[i]->join();

		}
	}
}


