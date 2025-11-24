#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <meta/base/core/Console.hpp>
#include <meta/gui/FontManager.hpp> // include the singleton font manager
#include <meta/gui/Theme.hpp>
#include <meta/gui/widgets/Widget.hpp>
#include <memory>

namespace meta::gui
{
    class Button : public Widget
    {
    public:
        Button(const meta::String<>& label = "", const std::shared_ptr<Theme>& theme = std::make_shared<Theme>())
            : Widget(theme->minWidth, theme->minHeight, 100, 30), m_label(label), m_theme(theme)
        {
            loadFont();
        }

        Signal<> clicked;

        void setTheme(const std::shared_ptr<Theme>& theme) override
        {
            m_theme = theme;
            loadFont();
        }

        void render(SDL_Renderer* renderer) override
        {
            if (!m_visible)
                return;

            const Theme& theme = m_theme ? *m_theme : DEFAULT_THEME;

            // Determine background color
            SDL_Color bgColor = theme.defaultWidgetColor;
            if (m_pressed)
                bgColor = theme.widgetPressedColor;
            else if (m_hovered)
                bgColor = theme.widgetHoverColor;

            // Draw button rectangle
            SDL_Rect rect{ m_x, m_y, m_width, m_height };
            drawRoundedRect(renderer, rect, bgColor, theme.borderRadius);

            // Draw label centered
            if (m_font && !m_label.empty())
            {
                SDL_Surface* surface = TTF_RenderText_Blended(m_font, m_label.c_str(), theme.widgetTextColor);
                if (surface)
                {
                    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                    int textW = surface->w;
                    int textH = surface->h;

                    SDL_Rect dst{ m_x + (m_width - textW) / 2, m_y + (m_height - textH) / 2, textW, textH };
                    SDL_RenderCopy(renderer, texture, nullptr, &dst);

                    SDL_DestroyTexture(texture);
                    SDL_FreeSurface(surface);
                }
            }

            drawOutline(renderer, theme);
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
                m_hovered = mx >= m_x && mx <= m_x + m_width && my >= m_y && my <= m_y + m_height;
            }

            if (m_hovered && e.type == SDL_MOUSEBUTTONDOWN)
                m_pressed = true;

            if (m_hovered && e.type == SDL_MOUSEBUTTONUP)
            {
                if (m_pressed)
                    clicked.emit();
                m_pressed = false;
            }

            if (e.type == SDL_MOUSEBUTTONUP && !m_hovered)
                m_pressed = false;
        }

        int getWidth() const override
        {
            const Theme& theme = m_theme ? *m_theme : DEFAULT_THEME;
            int textWidth = 0;
            if (m_font && !m_label.empty())
                TTF_SizeText(m_font, m_label.c_str(), &textWidth, nullptr);

            return std::max(m_width, textWidth + 2 * theme.padding);
        }

        int getHeight() const override
        {
            const Theme& theme = m_theme ? *m_theme : DEFAULT_THEME;
            int textHeight = m_font && !m_label.empty() ? TTF_FontHeight(m_font) : 0;
            return std::max(m_height, textHeight + 2 * theme.padding);
        }

    private:
        meta::String<> m_label;
        TTF_Font* m_font = nullptr;
        std::shared_ptr<Theme> m_theme;

        bool m_hovered = false;
        bool m_pressed = false;

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

        void drawRoundedRect(SDL_Renderer* renderer, const SDL_Rect& rect, const SDL_Color& color, int radius)
        {
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            // Placeholder: simple filled rect. Replace with SDL2_gfx or custom for rounded corners.
            SDL_RenderFillRect(renderer, &rect);
        }
    };
} // namespace meta::gui
