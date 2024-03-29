/*
 * io_context_pool.cpp
 *
 *  Created on: 2019��12��19��
 *      Author: poplarli
 */

#include <stdexcept>
#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>
#include "io_context_pool.hpp"


namespace http {
	namespace server{
		io_context_pool::io_context_pool(std::size_t pool_size) : next_io_context_(0){
			if(pool_size == 0){
				throw std::runtime_error("io_context_pool size is 0");
			}

			for(auto i = 0; i != pool_size; ++i){
				io_context_ptr io_context(new boost::asio::io_context());
				io_contexts_.push_back(io_context);
				work_.push_back(boost::asio::make_work_guard(*io_context));
			}
		}

		void io_context_pool::run(){
/*
			std::vector<boost::shared_ptr<boost::thread> > threads;
			for(auto ioc : io_contexts_){
				boost::shared_ptr<boost::thread> thread(new boost::thread(
					  boost::bind(&boost::asio::io_context::run, ioc)));
				threads.push_back(thread);
			}
*/
			std::vector<std::shared_ptr<std::thread>> threads;
			for(auto ioc : io_contexts_){
				threads.push_back(std::make_shared<std::thread>([ioc]{
						ioc->run();
				}));
			}


			// Wait for all threads in the pool to exit.
			for (std::size_t i = 0; i < threads.size(); ++i){
				threads[i]->join();
			}

		}

		void io_context_pool::stop(){
			 // Explicitly stop all io_contexts.
			  for (std::size_t i = 0; i < io_contexts_.size(); ++i)
			    io_contexts_[i]->stop();
		}

		boost::asio::io_context& io_context_pool::get_io_context()
		{
		  // Use a round-robin scheme to choose the next io_context to use.
		  boost::asio::io_context& io_context = *io_contexts_[next_io_context_];
		  ++next_io_context_;
		  if (next_io_context_ == io_contexts_.size())
		    next_io_context_ = 0;
		  return io_context;
		}

	}
}  // namespace http


