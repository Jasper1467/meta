#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <functional>
#include <meta/base/core/Console.hpp>
#include <meta/gui/Theme.hpp>
#include <meta/gui/Widget.hpp>

namespace meta::gui
{
    class Button : public Widget
    {
    public:
        Button(const meta::String<>& label = "", const std::shared_ptr<Theme>& theme = std::make_shared<Theme>())
            : Widget(0, 0, 100, 30), m_label(label), m_theme(theme)
        {
            if (TTF_WasInit() == 0)
            {
                if (TTF_Init() == -1)
                    meta::errorln("Failed to initialize SDL_ttf: ", TTF_GetError());
            }

            if (!m_theme->fontPath.empty())
            {
                m_font = TTF_OpenFont(m_theme->fontPath.c_str(), m_theme->fontSize);
                if (!m_font)
                    meta::errorln("Failed to load font: ", TTF_GetError());
            }
            else
            {
                meta::errorln("No font path specified in theme.");
            }
        }
        void setOnClick(std::function<void()> callback)
        {
            m_onClick = callback;
        }

        void setTheme(const std::shared_ptr<Theme>& theme) override
        {
            m_theme = theme;

            if (m_font)
            {
                TTF_CloseFont(m_font);
                m_font = nullptr;
            }

            if (!m_theme->fontPath.empty())
            {
                m_font = TTF_OpenFont(m_theme->fontPath.c_str(), m_theme->fontSize);
                if (!m_font)
                    meta::errorln("Failed to load font: ", TTF_GetError());
            }
        }

        void render(SDL_Renderer* renderer) override
        {
            if (!m_visible)
                return;

            // Use assigned theme or fall back to DEFAULT_THEME
            const Theme& theme = m_theme ? *m_theme : DEFAULT_THEME;

            // Determine background color based on state
            SDL_Color bgColor = theme.defaultWidgetColor;
            if (m_pressed)
                bgColor = theme.widgetPressedColor;
            else if (m_hovered)
                bgColor = theme.widgetHoverColor;

            // Draw button rectangle
            SDL_Rect rect{ m_x, m_y, m_width, m_height };
            SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
            SDL_RenderFillRect(renderer, &rect);

            // Draw border
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &rect);

            // Draw label
            if (m_font && !m_label.empty())
            {
                SDL_Color textColor = theme.widgetTextColor;
                SDL_Surface* surface = TTF_RenderText_Blended(m_font, m_label.c_str(), textColor);
                if (!surface)
                    return;

                SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                if (!texture)
                {
                    SDL_FreeSurface(surface);
                    return;
                }

                int textW = 0, textH = 0;
                SDL_QueryTexture(texture, nullptr, nullptr, &textW, &textH);

                SDL_Rect dst{ m_x + (m_width - textW) / 2, m_y + (m_height - textH) / 2, textW, textH };
                SDL_RenderCopy(renderer, texture, nullptr, &dst);

                SDL_DestroyTexture(texture);
                SDL_FreeSurface(surface);
            }
        }

        void handleEvent(const SDL_Event& e) override
        {
            if (!m_visible)
                return;

            int mx, my;
            if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP)
            {
                mx = e.motion.x;
                my = e.motion.y;
                m_hovered = mx >= m_x && mx <= m_x + m_width && my >= m_y && my <= m_y + m_height;
            }

            if (m_hovered && e.type == SDL_MOUSEBUTTONDOWN)
                m_pressed = true;

            if (m_hovered && e.type == SDL_MOUSEBUTTONUP)
            {
                if (m_pressed && m_onClick)
                    m_onClick();
                m_pressed = false;
            }

            if (e.type == SDL_MOUSEBUTTONUP && !m_hovered)
                m_pressed = false;
        }

    private:
        meta::String<> m_label;
        TTF_Font* m_font = nullptr;
        std::shared_ptr<Theme> m_theme;

        bool m_hovered = false;
        bool m_pressed = false;
        std::function<void()> m_onClick;
    };
} // namespace meta::gui
