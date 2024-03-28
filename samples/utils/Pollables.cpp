#include "Pollables.h"

namespace MinimalSocket::samples {
void Pollables::loop(const std::chrono::seconds &timeout) {
  auto last_notable_event = std::chrono::high_resolution_clock::now();
  while (true) {
    auto it = pollables_.begin();
    while (it != pollables_.end()) {
      auto stat = (*it)();
      if (stat != PollableStatus::NOT_ADVANCED) {
        last_notable_event = std::chrono::high_resolution_clock::now();
      }
      if (stat == PollableStatus::COMPLETED) {
        it = pollables_.erase(it);
        continue;
      }
      ++it;
    }

    auto elapsed_since_last_notable =
        std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::high_resolution_clock::now() - last_notable_event);
    if (timeout < elapsed_since_last_notable) {
      break;
    }
  }
}

} // namespace MinimalSocket::samples
