#pragma once

#include <chrono>
#include <iostream>
#include <meta/base/core/Platform.hpp>
#include <string_view>

namespace meta
{
    // Tag types for durations
    struct Seconds
    {
    };
    struct Milliseconds
    {
    };
    struct Microseconds
    {
    };
    struct Nanoseconds
    {
    };

    template <typename DurationTag = Milliseconds> class META_ALIGN(8) Timer
    {
    public:
        using Clock = std::chrono::high_resolution_clock;

        META_INLINE Timer() noexcept
        {
            reset();
        }

        // Start / reset timer
        META_FORCE_INLINE void reset() noexcept
        {
            m_start = Clock::now();
        }

        // Get elapsed time in selected duration
        META_FORCE_INLINE double elapsed() const noexcept
        {
            auto end = Clock::now();
            return to_duration(end - m_start);
        }

        // Print elapsed time with optional label
        META_INLINE void print(std::string_view label = {}) const noexcept
        {
            if (!label.empty())
                std::cout << label << ": ";
            std::cout << elapsed() << " " << duration_name() << "\n";
        }

    private:
        Clock::time_point m_start;

        META_FORCE_INLINE constexpr double to_duration(std::chrono::high_resolution_clock::duration d) const noexcept
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
                return std::chrono::duration<double, std::milli>(d).count();
        }

        META_INLINE constexpr const char* duration_name() const noexcept
        {
            if constexpr (std::is_same_v<DurationTag, Seconds>)
                return "s";
            if constexpr (std::is_same_v<DurationTag, Milliseconds>)
                return "ms";
            if constexpr (std::is_same_v<DurationTag, Microseconds>)
                return "us";
            if constexpr (std::is_same_v<DurationTag, Nanoseconds>)
                return "ns";

            return "ms";
        }
    };

    // Convenience typedefs
    using TimerS = Timer<Seconds>;
    using TimerMs = Timer<Milliseconds>;
    using TimerUs = Timer<Microseconds>;
    using TimerNs = Timer<Nanoseconds>;
} // namespace meta
