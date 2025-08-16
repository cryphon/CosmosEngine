// ==
// Standard Library
// ==

// ==
// Third Party
// ==

// ==
// Cosmos
// ==
#include <cosmos/core/Profiler.hpp>
#include <algorithm>

namespace cosmos::core {

FrameStats& Profiler::begin_frame() {
    if(frame_open_) {
        end_frame();
    }
    current_ = FrameStats{};    // reset fields
    current_.frame_index = ++frame_counter_;

    cpu_start_time_ = clock::now();
    frame_open_ = true;

    return current_;
}

void Profiler::end_frame() {
    if(!frame_open_) {
        return;
    }

    auto t1 = clock::now();
    current_.cpu_frame_ms = 
        std::chrono::duration<double, std::milli>(t1 - cpu_start_time_).count();

    // push to hist and cap size
    history_.push_back(current_);
    if (history_.size() > max_history_) {
        history_.pop_front();
    }

    frame_open_ = false;
}

void Profiler::set_max_history(std::size_t n) {
  max_history_ = std::max<std::size_t>(1, n);
  while (history_.size() > max_history_) {
    history_.pop_front();
  }
}



}
