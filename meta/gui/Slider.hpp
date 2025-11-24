#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <meta/base/core/Console.hpp>
#include <meta/base/core/String.hpp>
#include <meta/gui/Theme.hpp>
#include <meta/gui/Widget.hpp>
#include <meta/gui/FontManager.hpp>

namespace meta::gui
{
    class Slider : public Widget
    {
    public:
        Slider(const meta::String<>& name = "", int min = 0, int max = 100, int value = 0)
            : Widget(DEFAULT_THEME.minWidth, DEFAULT_THEME.minHeight, 200, 30),
              m_name(name), m_min(min), m_max(max), m_value(value)
        {
            loadFont();
        }

        // Signal for value changes
        meta::Signal<int> valueChanged;

        void setTheme(const std::shared_ptr<Theme>& theme) override
        {
            m_theme = theme;
            loadFont();
        }

        void setValue(int val)
        {
            int clamped = std::clamp(val, m_min, m_max);
            if (clamped != m_value)
            {
                m_value = clamped;
                valueChanged.emit(m_value); // emit signal on value change
            }
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

            int labelHeight = 0;
            if (m_font && !m_name.empty())
            {
                SDL_Surface* surface = TTF_RenderText_Blended(m_font, m_name.c_str(), theme.widgetTextColor);
                if (surface)
                {
                    labelHeight = surface->h + theme.textSpacing;
                    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                    SDL_Rect dst{ m_x, m_y, surface->w, surface->h };
                    SDL_RenderCopy(renderer, texture, nullptr, &dst);
                    SDL_DestroyTexture(texture);
                    SDL_FreeSurface(surface);
                }
            }

            // Slider bar rectangle
            SDL_Rect barRect{ m_x + theme.padding, m_y + labelHeight + theme.padding, m_width - 2 * theme.padding, 6 };
            drawRect(renderer, barRect, theme.defaultWidgetColor);

            // Draw knob
            int knobX = barRect.x + (m_value - m_min) * (barRect.w - 10) / (m_max - m_min);
            SDL_Rect knobRect{ knobX, barRect.y - 2, 10, 10 };
            drawRect(renderer, knobRect, theme.widgetPressedColor);

            drawOutline(renderer, theme);
        }

        void handleEvent(const SDL_Event& e) override
        {
            if (!m_visible)
                return;

            const Theme& theme = m_theme ? *m_theme : DEFAULT_THEME;
            int labelHeight = m_font && !m_name.empty() ? TTF_FontHeight(m_font) + theme.textSpacing : 0;

            SDL_Rect barRect{ m_x + theme.padding, m_y + labelHeight + theme.padding, m_width - 2 * theme.padding, 6 };

            if (e.type == SDL_MOUSEBUTTONDOWN || (e.type == SDL_MOUSEMOTION && m_dragging))
            {
                int mx = e.motion.x;
                int my = e.motion.y;

                if (mx >= barRect.x && mx <= barRect.x + barRect.w && my >= barRect.y && my <= barRect.y + barRect.h)
                {
                    m_dragging = true;
                    setValue(m_min + (mx - barRect.x) * (m_max - m_min) / barRect.w); // use setValue to emit signal
                }
            }

            if (e.type == SDL_MOUSEBUTTONUP)
                m_dragging = false;
        }

        int getWidth() const override
        {
            const Theme& theme = m_theme ? *m_theme : DEFAULT_THEME;
            int labelWidth = 0;
            if (m_font && !m_name.empty())
                TTF_SizeText(m_font, m_name.c_str(), &labelWidth, nullptr);

            return std::max(m_width, labelWidth + 2 * theme.padding);
        }

        int getHeight() const override
        {
            const Theme& theme = m_theme ? *m_theme : DEFAULT_THEME;
            int labelHeight = m_font && !m_name.empty() ? TTF_FontHeight(m_font) + theme.textSpacing : 0;
            return labelHeight + 6 + 2 * theme.padding;
        }

    private:
        meta::String<> m_name;
        int m_min, m_max;
        int m_value;
        bool m_dragging = false;

        TTF_Font* m_font = nullptr;
        std::shared_ptr<Theme> m_theme;

        void drawRect(SDL_Renderer* renderer, const SDL_Rect& rect, const SDL_Color& color)
        {
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            SDL_RenderFillRect(renderer, &rect);
        }

        void loadFont()
        {
            if (m_theme && !m_theme->fontPath.empty())
            {
                m_font = FontManager::instance().loadFont(m_theme->fontPath.c_str(), m_theme->fontSize);
                if (!m_font)
                    meta::errorln("Failed to load font from FontManager!");
            }
            else
            {
                meta::errorln("No font path specified in theme.");
            }
        }
    };
} // namespace meta::gui

