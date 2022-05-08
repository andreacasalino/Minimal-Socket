/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <string>

namespace MinimalSocket {
/**
 * @brief returns the last error code raised by the socket API
 */
int getLastErrorCode();

void throwWithLastErrorCode(const std::string &what);
} // namespace MinimalSocket
