/*
 * header.hpp
 *
 *  Created on: 2019��12��18��
 *      Author: poplarli
 */

#ifndef HEADER_HPP_
#define HEADER_HPP_

#include <string>

namespace http{

	namespace server {

		struct header{
			std::string name;
			std::string value;
		};

	}  // namespace name
};



#endif /* HEADER_HPP_ */
