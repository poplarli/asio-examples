/*
 * mime_types.hpp
 *
 *  Created on: 2019Äê12ÔÂ18ÈÕ
 *      Author: poplarli
 */

#ifndef MIME_TYPES_HPP_
#define MIME_TYPES_HPP_

#include <string>

namespace http {
namespace server {
namespace mime_types {

/// Convert a file extension into a MIME type.
std::string extension_to_type(const std::string& extension);

} // namespace mime_types
} // namespace server
} // namespace http




#endif /* MIME_TYPES_HPP_ */
