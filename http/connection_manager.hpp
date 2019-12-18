/*
 * connection_manager.hpp
 *
 *  Created on: 2019Äê12ÔÂ18ÈÕ
 *      Author: poplarli
 */

#ifndef CONNECTION_MANAGER_HPP_
#define CONNECTION_MANAGER_HPP_

#include <set>
#include "connection.hpp"

namespace http {
namespace server {
	class connection_manager{
		public:
		  connection_manager(const connection_manager&) = delete;
		  connection_manager& operator=(const connection_manager&) = delete;
		  connection_manager();

		  void start(connection_ptr c);
		  void stop(connection_ptr c);
		  void stop_all();

		private:

		  std::set<connection_ptr> connections_;


	};
}
}



#endif /* CONNECTION_MANAGER_HPP_ */
