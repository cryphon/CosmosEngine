#pragma once
// ==
// Standard Library
// ==
#include <deque>

// ==
// Third Party
// ==

// ==
// Cosmos
// ==
#include <cosmos/core/AppAPI.hpp>

namespace cosmos::core {

class Profiler {
public:
    explicit Profiler(std::size_t max_hist = 240)
        : max_history_(max_hist) {}


    FrameStats& begin_frame(); // resets and returns writable stats
    void end_frame(); // finalize (push into buffer)

    void set_max_history(size_t n);
    const std::deque<FrameStats>& history() const { return history_; }
    FrameStats& current() { return current_; }
    const FrameStats& current() const { return current_; }

private:
    using clock = std::chrono::high_resolution_clock;

    std::size_t max_history_;
    std::deque<FrameStats> history_;

    FrameStats current_{};
    std::uint64_t frame_counter_ = 0;

    clock::time_point cpu_start_time_{};
    bool frame_open_ = false;
};
}
