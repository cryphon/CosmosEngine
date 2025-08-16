#pragma once

#include <imgui.h>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cmath>

#include <cosmos/core/Profiler.hpp> // FrameStats, Profiler

namespace cosmos::ui {

struct PerfOverlayConfig {
    // Overlay window
    bool  show            = true;
    bool  lock_top_right  = false;   // position in top-right corner
    float alpha           = 0.95f;  // window transparency
    ImVec2 pivot          = ImVec2(1.0f, 0.0f); // top-right pivot for positioning

    // Graph
    int   graph_len       = 320;    // how many frames to display
    float graph_height    = 80.0f;  // pixels
    bool  graph_log_scale = false;  // log scale for frame time

    // Thresholds (ms) for coloring
    float good_ms         = 8.0f;   // ~120 FPS
    float ok_ms           = 16.67f; // ~60 FPS
};

namespace detail {

// format helpers
inline const char* fmt_bytes(uint64_t b, char out[32]) {
    const char* units[] = {"B", "KB", "MB", "GB", "TB"};
    double v = static_cast<double>(b);
    int u = 0;
    while (v >= 1024.0 && u < 4) { v /= 1024.0; ++u; }
    std::snprintf(out, 32, "%.2f %s", v, units[u]);
    return out;
}

inline ImU32 severity_color(float ms, float good_ms, float ok_ms) {
    if (ms <= good_ms) return IM_COL32( 80, 200, 120, 255); // green
    if (ms <= ok_ms)   return IM_COL32(255, 196,   0, 255); // yellow
    return               IM_COL32(235,  87,  87, 255);      // red
}

inline float to_log(float x) {
    // simple perceptual scaling for frame times; avoids -inf at 0
    return std::log10(std::max(0.0001f, x));
}

} // namespace detail

inline void draw_perf_overlay(const cosmos::core::Profiler& profiler, PerfOverlayConfig& cfg) {
    if (!cfg.show) return;

    ImGuiIO& io = ImGui::GetIO();
    ImGuiViewport* vp = ImGui::GetMainViewport();
    ImVec2 pad = ImGui::GetStyle().WindowPadding;

    // Window position (top-right by default)
    if (cfg.lock_top_right) {
        ImVec2 pos = ImVec2(vp->Pos.x + vp->Size.x - pad.x, vp->Pos.y + pad.y);
        ImGui::SetNextWindowPos(pos, ImGuiCond_Always, cfg.pivot);
    }
    ImGui::SetNextWindowBgAlpha(cfg.alpha);

    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoSavedSettings;
    if(cfg.lock_top_right) flags |= ImGuiWindowFlags_NoMove;

    if (!ImGui::Begin("Performance", nullptr, flags)) {
        ImGui::End();
        return;
    }

    // ---- Current frame stats
    const cosmos::core::FrameStats* sp = nullptr;
    if (!profiler.history().empty()) {
        sp = &profiler.history().back();
    } 
    else {
        sp = &profiler.current();
    }
    const auto& s = *sp;


    // Header: FPS + frame ms
    const float fps = (s.frame_ms > 0.0) ? (1000.0f / static_cast<float>(s.frame_ms)) : 0.0f;
    ImU32 col = detail::severity_color(static_cast<float>(s.frame_ms), cfg.good_ms, cfg.ok_ms);

    ImGui::Text("FPS: ");
    ImGui::SameLine();
    ImGui::TextColored(ImColor(col), "%.1f", fps);
    ImGui::SameLine();
    ImGui::Text("| Frame: %.2f ms", s.frame_ms);

    // CPU/GPU breakdown (side-by-side)
    ImGui::Separator();
    ImGui::Text("CPU frame: %.2f ms", s.cpu_frame_ms);
    ImGui::SameLine();
    if (s.gpu_frame_ms > 0.0) {
        ImGui::Text("GPU frame: %.2f ms", s.gpu_frame_ms);
    } else {
        ImGui::TextDisabled("GPU frame: n/a");
    }

    // Timings table
    if (ImGui::BeginTable("##timings", 3, ImGuiTableFlags_SizingStretchProp)) {
        ImGui::TableSetupColumn("Stage");
        ImGui::TableSetupColumn("ms");
        ImGui::TableSetupColumn("% CPU");
        ImGui::TableHeadersRow();

        auto row = [&](const char* name, double ms) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::TextUnformatted(name);
            ImGui::TableSetColumnIndex(1); ImGui::Text("%.2f", ms);
            ImGui::TableSetColumnIndex(2);
            if (s.cpu_frame_ms > 0.0) {
                ImGui::ProgressBar(static_cast<float>(ms / s.cpu_frame_ms), ImVec2(-FLT_MIN, 0), nullptr);
            } else {
                ImGui::TextDisabled("--");
            }
        };
        row("Input",           s.input_ms);
        row("Update",          s.update_ms);
        row("Render Submit",   s.render_submit_ms);
        ImGui::EndTable();
    }

    // ---- History graph (frame ms)
    const auto& hist = profiler.history();
    if (!hist.empty()) {
        int n = std::min<int>(cfg.graph_len, static_cast<int>(hist.size()));
        std::vector<float> y; y.reserve(n);

        // take last n frames
        for (int i = static_cast<int>(hist.size()) - n; i < static_cast<int>(hist.size()); ++i) {
            float v = static_cast<float>(hist[i].frame_ms);
            y.push_back(cfg.graph_log_scale ? detail::to_log(v) : v);
        }

        // graph labels
        float min_v = *std::min_element(y.begin(), y.end());
        float max_v = *std::max_element(y.begin(), y.end());
        if (!cfg.graph_log_scale) {
            // pad max to nearest multiple for nicer scale
            max_v = std::max(max_v, cfg.ok_ms);
        }

        ImGui::Separator();
        ImGui::Text("Frame Time (last %d)", n);
        ImGui::PlotLines("##frametimes", y.data(), n, 0, nullptr, min_v, max_v, ImVec2(280.0f, cfg.graph_height));

        // 1% low / 0.1% low (approx) from history
        std::vector<float> sorted = y;
        std::sort(sorted.begin(), sorted.end());
        auto pct = [&](double p) {
            if (sorted.empty()) return 0.0f;
            double idx = p * (sorted.size() - 1);
            return sorted[static_cast<int>(idx)];
        };
        float p99 = pct(0.99), p999 = pct(0.999);
        if (!cfg.graph_log_scale) {
            ImGui::TextDisabled("p99: %.2f ms  |  p99.9: %.2f ms", p99, p999);
        } else {
            ImGui::TextDisabled("log scale active");
        }
    }

    // ---- Renderer counters
    if (s.draw_calls || s.shader_binds || s.texture_binds || s.fbo_binds) {
        ImGui::Separator();
        ImGui::Text("Renderer");
        if (ImGui::BeginTable("##renderer", 4, ImGuiTableFlags_SizingStretchProp)) {
            ImGui::TableSetupColumn("Draw Calls");
            ImGui::TableSetupColumn("Program Binds");
            ImGui::TableSetupColumn("Tex Binds");
            ImGui::TableSetupColumn("FBO Binds");
            ImGui::TableHeadersRow();

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("%d", s.draw_calls);
            ImGui::TableSetColumnIndex(1); ImGui::Text("%d", s.shader_binds);
            ImGui::TableSetColumnIndex(2); ImGui::Text("%d", s.texture_binds);
            ImGui::TableSetColumnIndex(3); ImGui::Text("%d", s.fbo_binds);
            ImGui::EndTable();
        }
    }

    // ---- Scene
    if (s.entities_total || s.entities_visible || s.lights) {
        ImGui::Separator();
        ImGui::Text("Scene");
        ImGui::Text("Entities: %d visible / %d total", s.entities_visible, s.entities_total);
        ImGui::SameLine();
        ImGui::Text("| Lights: %d", s.lights);
    }

    // ---- Memory
    if (s.vram_textures || s.vram_buffers || s.vram_fbos || s.cpu_assets_bytes) {
        ImGui::Separator();
        ImGui::Text("Memory");
        char a[32], b[32], c[32], d[32];
        ImGui::Text("VRAM  tex: %s", detail::fmt_bytes(s.vram_textures, a));
        ImGui::SameLine();
        ImGui::Text("| buf: %s", detail::fmt_bytes(s.vram_buffers, b));
        ImGui::SameLine();
        ImGui::Text("| fbo: %s", detail::fmt_bytes(s.vram_fbos, c));
        ImGui::Text("CPU assets: %s", detail::fmt_bytes(s.cpu_assets_bytes, d));
    }

    // Options (tiny)
    ImGui::Separator();
    ImGui::Checkbox("Log scale", &cfg.graph_log_scale);
    ImGui::SameLine();
    ImGui::TextDisabled("(%s)", cfg.lock_top_right ? "locked top-right" : "free move");

    ImGui::End();
}

// Optional: a movable, full “Performance Panel” with more room/details.
// You can call this from a Debug menu.
inline void DrawPerformancePanel(const cosmos::core::Profiler& profiler) {
    if (!ImGui::Begin("Performance Panel")) { ImGui::End(); return; }

    const auto& s = profiler.current();
    ImGui::Text("Frame #%llu", static_cast<unsigned long long>(s.frame_index));
    ImGui::Separator();

    // Reuse overlay content by calling with unlocked config (if you prefer)
    PerfOverlayConfig cfg;
    cfg.lock_top_right = false;
    cfg.graph_len = 360;
    cfg.graph_height = 120.0f;
    draw_perf_overlay(profiler, cfg);

    ImGui::End();
}

} // namespace cosmos::ui

