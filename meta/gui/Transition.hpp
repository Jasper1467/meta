#pragma once
#include <meta/base/core/Platform.hpp>
#include <meta/base/core/Timer.hpp>

namespace meta::gui
{
    class Transition
    {
    public:
        META_INLINE Transition(float start = 0.0f, float end = 1.0f, float durationSeconds = 0.2f)
            : m_start(start), m_end(end), m_duration(durationSeconds)
        {
            reset();
        }

        META_FORCE_INLINE void reset()
        {
            m_timer.reset();
            m_active = true;
        }

        META_FORCE_INLINE void setRange(float start, float end)
        {
            m_start = start;
            m_end = end;
            reset();
        }

        META_FORCE_INLINE void setDuration(float seconds)
        {
            m_duration = seconds;
            reset();
        }

        // Update the transition and get interpolated value
        META_FORCE_INLINE float update()
        {
            if (!m_active)
                return m_end;

            float elapsedMs = static_cast<float>(m_timer.elapsed()); // Timer in ms
            float t = elapsedMs / (m_duration * 1000.0f);            // duration converted to ms

            if (t >= 1.0f)
            {
                t = 1.0f;
                m_active = false;
            }

            return m_start + (m_end - m_start) * easeInOut(t);
        }

        META_INLINE bool isActive() const
        {
            return m_active;
        }

    private:
        float m_start;
        float m_end;
        float m_duration; // seconds
        bool m_active = false;
        meta::Timer<meta::Milliseconds> m_timer;

        // Smoothstep easing
        META_FORCE_INLINE static float easeInOut(float t)
        {
            return t * t * (3.0f - 2.0f * t);
        }
    };
} // namespace meta::gui
