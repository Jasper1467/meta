#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <meta/base/core/String.hpp>
#include <meta/gui/Theme.hpp>
#include <meta/gui/Widget.hpp>

namespace meta::gui
{
    class Tab : public Widget
    {
    public:
        Tab(const meta::String<>& label = "") : m_label(label)
        {
        }

        // Signal emitted when this tab is clicked
        Signal<> onSelect;

        void setTheme(const std::shared_ptr<Theme>& theme) override
        {
            m_theme = theme;
        }

        void setFont(TTF_Font* font)
        {
            m_font = font;
        }

        const meta::String<>& getLabel() const
        {
            return m_label;
        }

        void render(SDL_Renderer* renderer) override
        {
            if (!m_visible)
                return;

            const Theme& theme = m_theme ? *m_theme : DEFAULT_THEME;

            // Background
            SDL_Color bgColor = m_hovered ? theme.widgetHoverColor : theme.defaultWidgetColor;
            SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
            SDL_Rect rect{ m_x, m_y, m_width, m_height };
            SDL_RenderFillRect(renderer, &rect);

            // Outline
            drawOutline(renderer, theme);

            // Draw text centered
            if (!m_label.empty() && m_font)
            {
                SDL_Surface* surf = TTF_RenderText_Blended(m_font, m_label.c_str(), theme.widgetTextColor);
                if (surf)
                {
                    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
                    SDL_Rect dst{ m_x + (m_width - surf->w) / 2, m_y + (m_height - surf->h) / 2, surf->w, surf->h };
                    SDL_RenderCopy(renderer, tex, nullptr, &dst);
                    SDL_DestroyTexture(tex);
                    SDL_FreeSurface(surf);
                }
            }
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
                    onSelect.emit();
                m_pressed = false;
            }

            if (e.type == SDL_MOUSEBUTTONUP && !m_hovered)
                m_pressed = false;
        }

    private:
        meta::String<> m_label;
        std::shared_ptr<Theme> m_theme;
        TTF_Font* m_font = nullptr;
        bool m_hovered = false;
        bool m_pressed = false;
    };
} // namespace meta::gui
