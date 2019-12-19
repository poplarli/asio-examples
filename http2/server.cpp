/*
 * server.cpp
 *
 *  Created on: 2019��12��19��
 *      Author: poplarli
 */

#include "server.hpp"
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>


namespace http{
	namespace server{

	using boost::asio::ip::tcp;

		server::server(const std::string& address, const std::string& port,
							  const std::string& doc_root, std::size_t io_context_pool_size) :
		pool_(io_context_pool_size),
		signals_(pool_.get_io_context()),
		acceptor_(pool_.get_io_context()),
		new_connection_(),
		request_handler_(doc_root){
				signals_.add(SIGINT);
				signals_.add(SIGTERM);
				#if defined(SIGQUIT)
				signals_.add(SIGQUIT);
				#endif // defined(SIGQUIT

				do_await_stop();

				boost::asio::ip::tcp::resolver resolver(acceptor_.get_executor().context());
				boost::asio::ip::tcp::endpoint endpoint =
				*resolver.resolve(address, port).begin();
				acceptor_.open(endpoint.protocol());
				acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
				acceptor_.bind(endpoint);
				acceptor_.listen();

				do_accept();
			}

			void server::run()
			{
				pool_.run();
			}

			void server::do_await_stop(){
				signals_.async_wait([this](boost::system::error_code /*ec*/, int /*signo*/){
					pool_.stop();
				});
			}

			void server::do_accept(){

				//����һ��io_context���󡣸����ӵ������첽�¼����ڸ�io_context���е��߳��з���
				new_connection_.reset(new connection(pool_.get_io_context(), request_handler_));
				acceptor_.async_accept(new_connection_->socket(), [this](const boost::system::error_code& ec){

			        if (!acceptor_.is_open())
			        {
			          return;
			        }

			        if(!ec){
			        	new_connection_->start();
			        }
			        //һ���ͻ����������Ժ���ȥ������һ�����ӡ���������new_connection_��û����
			        do_accept();
				});
			}

		}


}
