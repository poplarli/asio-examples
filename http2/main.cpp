/*
 * main.cpp
 *
 *  Created on: 2019Äê12ÔÂ19ÈÕ
 *      Author: poplarli
 */

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include "server.hpp"


int main(int argc, char* argv[]){

	try{
		// Check command line arguments.
	    if (argc != 5)
	    {
	      std::cerr << "Usage: http_server <address> <port> <threads> <doc_root>\n";
	      std::cerr << "  For IPv4, try:\n";
	      std::cerr << "    receiver 0.0.0.0 80 1 .\n";
	      std::cerr << "  For IPv6, try:\n";
	      std::cerr << "    receiver 0::0 80 1 .\n";
	      return 1;
	    }

		http::server::server s(argv[1], argv[2], argv[4], atoi(argv[3]));

		// Run the server until stopped.
		s.run();

	}catch(std::exception& e){
		  std::cerr << "exception: " << e.what() << "\n";
	}


	return 0;
}



