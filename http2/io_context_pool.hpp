/*
 * io_context_pool.hpp
 *
 *  Created on: 2019Äê12ÔÂ19ÈÕ
 *      Author: poplarli
 */

#ifndef IO_CONTEXT_POOL_HPP_
#define IO_CONTEXT_POOL_HPP_

#include <boost/asio.hpp>
#include <list>
#include <vector>
#include <boost/shared_ptr.hpp>

namespace http {
	namespace server{
		class io_context_pool{
		public:
			io_context_pool(const io_context_pool&) = delete;
			io_context_pool& operator=(const io_context_pool&) = delete;
			io_context_pool(std::size_t pool_size);

			void run();
			void stop();
			boost::asio::io_context& get_io_context();

		private:
			using io_context_ptr = boost::shared_ptr<boost::asio::io_context>;
			using io_context_work = boost::asio::executor_work_guard<boost::asio::io_context::executor_type>;

			std::vector<io_context_ptr> io_contexts_;
			std::list<io_context_work> work_;
			size_t next_io_context_;
		};
	};

}  // namespace http



#endif /* IO_CONTEXT_POOL_HPP_ */
