#pragma once
#include <SDL.h>
#include <meta/gui/Theme.hpp>
#include <meta/gui/Widget.hpp>

namespace meta::gui
{
    class Toggle : public Widget
    {
    public:
        Toggle(bool initial = false) : Widget(0, 0, 40, 24), m_state(initial)
        {
        }

        void setTheme(const std::shared_ptr<Theme>& theme) override
        {
            m_theme = theme;
        }

        bool getState() const
        {
            return m_state;
        }
        void setState(bool s)
        {
            m_state = s;
        }

        void toggle()
        {
            m_state = !m_state;
        }

        void handleEvent(const SDL_Event& e) override
        {
            if (!m_visible)
                return;

            int mx = 0, my = 0;

            if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP)
            {
                mx = e.motion.x;
                my = e.motion.y;

                m_hovered = mx >= m_x && mx <= (m_x + m_width) && my >= m_y && my <= (m_y + m_height);
            }

            if (m_hovered && e.type == SDL_MOUSEBUTTONDOWN)
                m_pressed = true;

            if (m_pressed && m_hovered && e.type == SDL_MOUSEBUTTONUP)
            {
                toggle();
                m_pressed = false;
            }

            if (e.type == SDL_MOUSEBUTTONUP && !m_hovered)
                m_pressed = false;
        }

        void render(SDL_Renderer* renderer) override
        {
            if (!m_visible)
                return;

            const Theme& theme = m_theme ? *m_theme : DEFAULT_THEME;

            // Track dimensions
            const int trackH = theme.toggleTrackHeight;
            const int trackR = trackH / 2;
            const int trackW = m_width;

            SDL_Rect trackRect{ m_x, m_y + (m_height - trackH) / 2, trackW, trackH };

            // Colors
            SDL_Color trackColor = m_state ? theme.toggleOnColor : theme.toggleOffColor;
            SDL_Color knobColor = theme.toggleKnobColor;
            SDL_Color outlineColor = theme.toggleOutlineColor;

            // Draw track (rounded)
            drawRoundedRect(renderer, trackRect, trackR, trackColor);

            // Draw outline if enabled
            if (theme.widgetOutlineEnable && theme.toggleOutlineSize > 0)
                drawRoundedOutline(renderer, trackRect, trackR, outlineColor, theme.toggleOutlineSize);

            // Knob position
            const int margin = theme.toggleKnobMargin;
            const int knobDiameter = trackH - margin * 2;
            const int knobRadius = knobDiameter / 2;

            int knobX = m_state ? (trackRect.x + trackRect.w - knobDiameter - margin) : (trackRect.x + margin);

            int knobY = trackRect.y + margin;

            SDL_Rect knobRect{ knobX, knobY, knobDiameter, knobDiameter };

            drawCircle(renderer, knobRect, knobRadius, knobColor);

            // Optional knob outline
            if (theme.widgetOutlineEnable && theme.toggleOutlineSize > 0)
                drawCircleOutline(renderer, knobRect, knobRadius, outlineColor, theme.toggleOutlineSize);
        }

        int getHeight() const override
        {
            const Theme& theme = m_theme ? *m_theme : DEFAULT_THEME;
            return std::max(m_height, theme.toggleTrackHeight);
        }

    private:
        bool m_state = false;
        bool m_pressed = false;
        bool m_hovered = false;

        std::shared_ptr<Theme> m_theme;

        // --- Drawing helpers ---

        static void setColor(SDL_Renderer* r, SDL_Color c)
        {
            SDL_SetRenderDrawColor(r, c.r, c.g, c.b, c.a);
        }

        static void drawRoundedRect(SDL_Renderer* r, const SDL_Rect& rect, int radius, SDL_Color color)
        {
            setColor(r, color);

            // Middle rectangle
            SDL_Rect mid{ rect.x + radius, rect.y, rect.w - 2 * radius, rect.h };
            SDL_RenderFillRect(r, &mid);

            // Left
            SDL_Rect left{ rect.x, rect.y + radius, radius, rect.h - 2 * radius };
            SDL_RenderFillRect(r, &left);

            // Right
            SDL_Rect right{ rect.x + rect.w - radius, rect.y + radius, radius, rect.h - 2 * radius };
            SDL_RenderFillRect(r, &right);

            // Top-left circle
            drawFilledCircle(r, rect.x + radius, rect.y + radius, radius, color);

            // Top-right circle
            drawFilledCircle(r, rect.x + rect.w - radius, rect.y + radius, radius, color);

            // Bottom-left circle
            drawFilledCircle(r, rect.x + radius, rect.y + rect.h - radius, radius, color);

            // Bottom-right circle
            drawFilledCircle(r, rect.x + rect.w - radius, rect.y + rect.h - radius, radius, color);
        }

        static void drawRoundedOutline(SDL_Renderer* r, const SDL_Rect& rect, int radius, SDL_Color color, int size)
        {
            setColor(r, color);

            SDL_Rect outline = rect;

            for (int i = 0; i < size; i++)
            {
                SDL_RenderDrawRect(r, &outline);
                outline.x++;
                outline.y++;
                outline.w -= 2;
                outline.h -= 2;
            }
        }

        static void drawFilledCircle(SDL_Renderer* r, int cx, int cy, int radius, SDL_Color color)
        {
            setColor(r, color);
            for (int dy = -radius; dy <= radius; dy++)
                for (int dx = -radius; dx <= radius; dx++)
                    if (dx * dx + dy * dy <= radius * radius)
                        SDL_RenderDrawPoint(r, cx + dx, cy + dy);
        }

        static void drawCircle(SDL_Renderer* r, const SDL_Rect& rect, int radius, SDL_Color color)
        {
            drawFilledCircle(r, rect.x + radius, rect.y + radius, radius, color);
        }

        static void drawCircleOutline(SDL_Renderer* r, const SDL_Rect& rect, int radius, SDL_Color color, int size)
        {
            for (int i = 0; i < size; i++)
            {
                drawCircleOutlineSingle(r, rect.x + radius, rect.y + radius, radius - i, color);
            }
        }

        static void drawCircleOutlineSingle(SDL_Renderer* r, int cx, int cy, int radius, SDL_Color color)
        {
            setColor(r, color);

            int x = radius;
            int y = 0;
            int err = 0;

            while (x >= y)
            {
                SDL_RenderDrawPoint(r, cx + x, cy + y);
                SDL_RenderDrawPoint(r, cx + y, cy + x);
                SDL_RenderDrawPoint(r, cx - y, cy + x);
                SDL_RenderDrawPoint(r, cx - x, cy + y);
                SDL_RenderDrawPoint(r, cx - x, cy - y);
                SDL_RenderDrawPoint(r, cx - y, cy - x);
                SDL_RenderDrawPoint(r, cx + y, cy - x);
                SDL_RenderDrawPoint(r, cx + x, cy - y);

                y++;
                if (err <= 0)
                {
                    err += 2 * y + 1;
                }
                if (err > 0)
                {
                    x--;
                    err -= 2 * x + 1;
                }
            }
        }
    };
} // namespace meta::gui
