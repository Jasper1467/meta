#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <meta/base/core/Console.hpp>
#include <meta/base/core/String.hpp>
#include <meta/gui/Theme.hpp>
#include <meta/gui/Widget.hpp>

namespace meta::gui
{
    class Slider : public Widget
    {
    public:
        Slider(const meta::String<>& name = "", int min = 0, int max = 100, int value = 0)
            : Widget(0, 0, 200, 30), m_name(name), m_min(min), m_max(max), m_value(value)
        {
            if (TTF_WasInit() == 0)
                TTF_Init();

            m_font = TTF_OpenFont(DEFAULT_THEME.fontPath.c_str(), DEFAULT_THEME.fontSize);
            if (!m_font)
                meta::errorln("Failed to load font: ", TTF_GetError());
        }

        void setTheme(const std::shared_ptr<Theme>& theme) override
        {
            m_theme = theme;
        }

        void setValue(int val)
        {
            m_value = std::clamp(val, m_min, m_max);
        }

        int getValue() const
        {
            return m_value;
        }

        void render(SDL_Renderer* renderer) override
        {
            if (!m_visible)
                return;

            const Theme& theme = m_theme ? *m_theme : DEFAULT_THEME;

            // Calculate label size
            int labelHeight = 0;
            if (m_font && !m_name.empty())
            {
                SDL_Surface* surface = TTF_RenderText_Blended(m_font, m_name.c_str(), theme.widgetTextColor);
                if (surface)
                {
                    labelHeight = surface->h + 2; // add small spacing
                    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                    SDL_Rect dst{ m_x, m_y, surface->w, surface->h };
                    SDL_RenderCopy(renderer, texture, nullptr, &dst);
                    SDL_DestroyTexture(texture);
                    SDL_FreeSurface(surface);
                }
            }

            // Draw slider bar
            SDL_Rect barRect{ m_x, m_y + labelHeight, m_width, 6 };
            SDL_SetRenderDrawColor(renderer, theme.defaultWidgetColor.r, theme.defaultWidgetColor.g,
                                   theme.defaultWidgetColor.b, theme.defaultWidgetColor.a);
            SDL_RenderFillRect(renderer, &barRect);

            // Draw slider knob
            int knobX = m_x + (m_value - m_min) * (m_width - 10) / (m_max - m_min);
            SDL_Rect knobRect{ knobX, m_y + labelHeight - 2, 10, 10 };
            SDL_SetRenderDrawColor(renderer, theme.widgetPressedColor.r, theme.widgetPressedColor.g,
                                   theme.widgetPressedColor.b, theme.widgetPressedColor.a);
            SDL_RenderFillRect(renderer, &knobRect);
        }

        void handleEvent(const SDL_Event& e) override
        {
            if (!m_visible)
                return;

            if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEMOTION)
            {
                int mx = e.motion.x;
                int my = e.motion.y;

                // Simple hit test on slider bar
                int labelHeight = m_font ? TTF_FontHeight(m_font) + 2 : 0;
                SDL_Rect barRect{ m_x, m_y + labelHeight, m_width, 6 };
                if (mx >= barRect.x && mx <= barRect.x + barRect.w && my >= barRect.y && my <= barRect.y + barRect.h &&
                    (e.type == SDL_MOUSEBUTTONDOWN || (e.type == SDL_MOUSEMOTION && m_dragging)))
                {
                    m_dragging = true;
                    m_value = m_min + (mx - m_x) * (m_max - m_min) / m_width;
                }
            }

            if (e.type == SDL_MOUSEBUTTONUP)
            {
                m_dragging = false;
            }
        }

        int getWidth() const override
        {
            int labelWidth = 0;
            if (m_font && !m_name.empty())
            {
                int w;
                TTF_SizeText(m_font, m_name.c_str(), &w, nullptr);
                labelWidth = w;
            }
            return std::max(m_width, labelWidth);
        }

        int getHeight() const override
        {
            int labelHeight = 0;
            if (m_font && !m_name.empty())
                labelHeight = TTF_FontHeight(m_font) + 2;

            return labelHeight + 10; // slider bar height + knob
        }

    private:
        meta::String<> m_name;
        int m_min, m_max;
        int m_value;
        bool m_dragging = false;

        TTF_Font* m_font = nullptr;
        std::shared_ptr<Theme> m_theme;
    };
} // namespace meta::gui
