#include "Pollables.h"

namespace MinimalSocket::samples {
void Pollables::loop(const std::chrono::seconds &timeout) {
  auto last_notable_event = std::chrono::high_resolution_clock::now();
  while (!pollables_.empty()) {
    auto it = pollables_.begin();
    while (it != pollables_.end()) {
      auto stat = (*it)();
      switch (stat) {
      case PollableStatus::NOT_ADVANCED:
        ++it;
        break;
      case PollableStatus::ADVANCED:
        last_notable_event = std::chrono::high_resolution_clock::now();
        ++it;
        break;
      case PollableStatus::COMPLETED:
        last_notable_event = std::chrono::high_resolution_clock::now();
        it = pollables_.erase(it);
        break;
      }
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
