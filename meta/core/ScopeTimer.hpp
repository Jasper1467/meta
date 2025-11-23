#pragma once

#include "Timer.hpp"
#include <chrono>
#include <iostream>
#include <meta/core/Platform.hpp>
#include <string_view>

namespace meta
{
    template <typename DurationTag = Milliseconds> class ScopeTimer
    {
    public:
        META_INLINE explicit ScopeTimer(std::string_view label = {}) noexcept : m_label(label), m_start(Clock::now())
        {
        }

        // Delete copy/move to avoid accidental copies
        ScopeTimer(const ScopeTimer&) = delete;
        ScopeTimer& operator=(const ScopeTimer&) = delete;

        // On destruction, print elapsed time
        META_INLINE ~ScopeTimer() noexcept
        {
            auto end = Clock::now();
            double elapsed_time = to_seconds(end - m_start);

            if (!m_label.empty())
                std::cout << m_label << ": ";
            std::cout << elapsed_time << " " << duration_name() << "\n";
        }

    private:
        using Clock = std::chrono::high_resolution_clock;
        Clock::time_point m_start;
        std::string_view m_label;

        // Compile-time optimized conversion
        META_FORCE_INLINE constexpr double to_seconds(Clock::duration d) const noexcept
        {
            if constexpr (std::is_same_v<DurationTag, Seconds>)
                return std::chrono::duration<double>(d).count();
            else if constexpr (std::is_same_v<DurationTag, Milliseconds>)
                return std::chrono::duration<double, std::milli>(d).count();
            else if constexpr (std::is_same_v<DurationTag, Microseconds>)
                return std::chrono::duration<double, std::micro>(d).count();
            else if constexpr (std::is_same_v<DurationTag, Nanoseconds>)
                return std::chrono::duration<double, std::nano>(d).count();
            else
                return std::chrono::duration<double, std::milli>(d).count(); // default
        }

        META_INLINE constexpr const char* duration_name() const noexcept
        {
            if constexpr (std::is_same_v<DurationTag, Seconds>)
                return "s";
            else if constexpr (std::is_same_v<DurationTag, Milliseconds>)
                return "ms";
            else if constexpr (std::is_same_v<DurationTag, Microseconds>)
                return "us";
            else if constexpr (std::is_same_v<DurationTag, Nanoseconds>)
                return "ns";
            else
                return "ms";
        }
    };
} // namespace meta
