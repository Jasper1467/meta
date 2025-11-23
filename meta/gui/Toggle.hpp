#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <functional>
#include <meta/base/core/String.hpp>
#include <meta/gui/Theme.hpp>
#include <meta/gui/Transition.hpp>
#include <meta/gui/Widget.hpp>

namespace meta::gui
{
    class Toggle : public Widget
    {
    public:
        Toggle(const meta::String<>& label = "", bool initialState = false)
            : Widget(0, 0, 60, 28), // Desktop-friendly defaults: width=60, height=28
              m_label(label), m_state(initialState),
              m_knobTransition(initialState ? 1.0f : 0.0f, initialState ? 1.0f : 0.0f, 0.15f)
        {
        }

        void setTheme(const std::shared_ptr<Theme>& theme) override
        {
            m_theme = theme;
        }

        void setState(bool state)
        {
            if (state != m_state)
            {
                m_state = state;
                m_knobTransition.setRange(m_knobTransition.update(), m_state ? 1.0f : 0.0f);
                m_knobTransition.reset();
            }
        }

        bool getState() const
        {
            return m_state;
        }

        void render(SDL_Renderer* renderer) override
        {
            if (!m_visible)
                return;

            const Theme& theme = m_theme ? *m_theme : DEFAULT_THEME;
            float t = m_knobTransition.update();

            int width = m_width;
            int height = m_height;

            // Draw pill-shaped track
            SDL_Rect trackRect{ m_x, m_y, width, height };
            SDL_Color trackColor = {
                static_cast<Uint8>(theme.toggleOffColor.r + (theme.toggleOnColor.r - theme.toggleOffColor.r) * t),
                static_cast<Uint8>(theme.toggleOffColor.g + (theme.toggleOnColor.g - theme.toggleOffColor.g) * t),
                static_cast<Uint8>(theme.toggleOffColor.b + (theme.toggleOnColor.b - theme.toggleOffColor.b) * t), 255
            };
            filledRoundedRect(renderer, trackRect, height / 2, trackColor);

            // Draw knob
            int knobSize = height - 4; // internal padding
            int knobX = static_cast<int>(m_x + 2 + t * (width - knobSize - 4));
            SDL_Rect knobRect{ knobX, m_y + 2, knobSize, knobSize };
            filledCircle(renderer, knobRect.x + knobSize / 2, knobRect.y + knobSize / 2, knobSize / 2,
                         theme.toggleKnobColor);

            // Draw label (if any)
            if (!m_label.empty() && m_font)
            {
                SDL_Surface* surface = TTF_RenderText_Blended(m_font, m_label.c_str(), theme.labelTextColor);
                if (surface)
                {
                    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                    SDL_Rect dst{ m_x + width + theme.spacing, m_y + (height - surface->h) / 2, surface->w,
                                  surface->h };
                    SDL_RenderCopy(renderer, texture, nullptr, &dst);
                    SDL_DestroyTexture(texture);
                    SDL_FreeSurface(surface);
                }
            }
        }

        void handleEvent(const SDL_Event& e) override
        {
            if (!m_visible)
                return;

            if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                int mx = e.button.x;
                int my = e.button.y;
                if (mx >= m_x && mx <= m_x + m_width && my >= m_y && my <= m_y + m_height)
                {
                    setState(!m_state);
                    if (m_onToggle)
                        m_onToggle(m_state);
                }
            }
        }

        void setOnToggle(std::function<void(bool)> callback)
        {
            m_onToggle = callback;
        }

    private:
        meta::String<> m_label;
        bool m_state;
        Transition m_knobTransition;
        std::shared_ptr<Theme> m_theme;
        std::function<void(bool)> m_onToggle;
        TTF_Font* m_font = nullptr;

        void filledCircle(SDL_Renderer* renderer, int cx, int cy, int radius, SDL_Color color)
        {
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            for (int w = 0; w < radius * 2; ++w)
                for (int h = 0; h < radius * 2; ++h)
                    if ((radius - w) * (radius - w) + (radius - h) * (radius - h) <= radius * radius)
                        SDL_RenderDrawPoint(renderer, cx + (radius - w), cy + (radius - h));
        }

        void filledRoundedRect(SDL_Renderer* renderer, const SDL_Rect& rect, int radius, SDL_Color color)
        {
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

            // Center rectangle
            SDL_Rect center{ rect.x + radius, rect.y, rect.w - 2 * radius, rect.h };
            SDL_RenderFillRect(renderer, &center);

            // Side rectangles
            SDL_Rect left{ rect.x, rect.y + radius, radius, rect.h - 2 * radius };
            SDL_Rect right{ rect.x + rect.w - radius, rect.y + radius, radius, rect.h - 2 * radius };
            SDL_RenderFillRect(renderer, &left);
            SDL_RenderFillRect(renderer, &right);

            // Corners
            filledCircle(renderer, rect.x + radius, rect.y + radius, radius, color);
            filledCircle(renderer, rect.x + rect.w - radius, rect.y + radius, radius, color);
            filledCircle(renderer, rect.x + radius, rect.y + rect.h - radius, radius, color);
            filledCircle(renderer, rect.x + rect.w - radius, rect.y + rect.h - radius, radius, color);
        }
    };
} // namespace meta::gui
