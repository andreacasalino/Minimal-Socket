/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <functional>
#include <vector>

namespace MinimalSocket::test {
using Task = std::function<void()>;

void parallel(const Task &master, const Task &slave);

void parallel(const std::vector<Task> &tasks);
} // namespace MinimalSocket::test
