/*
 * connection_manager.cpp
 *
 *  Created on: 2019Äê12ÔÂ18ÈÕ
 *      Author: poplarli
 */

#include "connection_manager.hpp"

#include <iostream>

namespace http {
namespace server {
	connection_manager::connection_manager(){
	}

	void connection_manager::start(connection_ptr c){
	  connections_.insert(c);
	  std::cout << "connection size = " << connections_.size() << std::endl;
	  c->start();
	}

	void connection_manager::stop(connection_ptr c){
	  connections_.erase(c);
	  c->stop();
	  std::cout << "connection size = " << connections_.size() << std::endl;
	}

	void connection_manager::stop_all(){
	  for (auto c: connections_)
		  c->stop();
	  	  connections_.clear();
	}
}
}

