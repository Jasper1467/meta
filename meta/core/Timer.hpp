#pragma once

#include <chrono>
#include <iostream>
#include <meta/core/Platform.hpp>
#include <string_view>

namespace meta
{
    // Tag types for durations (compile-time)
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

    template <typename DurationTag = Milliseconds> class Timer
    {
    public:
        using Clock = std::chrono::high_resolution_clock;

        META_INLINE Timer() noexcept
        {
            reset();
        }

        // Start / reset the timer
        META_INLINE void reset() noexcept
        {
            m_start = Clock::now();
        }

        // Get elapsed time in the selected duration type
        META_INLINE double elapsed() const noexcept
        {
            auto end = Clock::now();
            return to_seconds(end - m_start);
        }

        // Print elapsed time with an optional label
        META_INLINE void print(std::string_view label = {}) const noexcept
        {
            if (!label.empty())
                std::cout << label << ": ";
            std::cout << elapsed() << " " << duration_name() << "\n";
        }

    private:
        Clock::time_point m_start;

        // Convert to seconds at compile-time for selected duration type
        META_FORCE_INLINE constexpr double to_seconds(std::chrono::high_resolution_clock::duration d) const noexcept
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

        // Duration name for printing
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
