/*
 * request.hpp
 *
 *  Created on: 2019Äê12ÔÂ18ÈÕ
 *      Author: poplarli
 */

#ifndef REQUEST_HPP_
#define REQUEST_HPP_

#include <string>
#include <vector>
#include "header.hpp"

namespace http {
namespace server {

/// A request received from a client.
struct request
{
  std::string method;
  std::string uri;
  int http_version_major;
  int http_version_minor;
  std::vector<header> headers;
};

} // namespace server
} // namespace http



#endif /* REQUEST_HPP_ */
