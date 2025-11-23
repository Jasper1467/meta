#pragma once

#include <SDL.h>
#include <SDL_ttf.h>

#include <meta/base/core/Console.hpp>
#include <meta/gui/Theme.hpp>
#include <meta/gui/Widget.hpp>

namespace meta::gui
{
    class CheckBox : public Widget
    {
    public:
        CheckBox(const meta::String<>& label = "", bool checked = false,
                 const std::shared_ptr<Theme>& theme = std::make_shared<Theme>())
            : m_label(label), m_checked(checked), m_theme(theme)
        {
            if (TTF_WasInit() == 0 && TTF_Init() == -1)
                meta::errorln("Failed to initialize SDL_ttf: ", TTF_GetError());

            if (!m_theme->fontPath.empty())
            {
                m_font = TTF_OpenFont(m_theme->fontPath.c_str(), m_theme->fontSize);
                if (!m_font)
                    meta::errorln("Failed to load font: ", TTF_GetError());
            }

            m_width = 120;
            m_height = m_theme->minHeight;
        }

        ~CheckBox()
        {
            if (m_font)
                TTF_CloseFont(m_font);
        }

        // Signal for check state changes
        meta::Signal<bool> stateChanged;

        void setChecked(bool value)
        {
            if (m_checked != value)
            {
                m_checked = value;
                stateChanged.emit(m_checked); // emit signal on change
            }
        }

        bool isChecked() const
        {
            return m_checked;
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

            const Theme& theme = m_theme ? *m_theme : DEFAULT_THEME;

            int boxSize = m_height - theme.padding * 2;
            if (boxSize < 14)
                boxSize = 14;

            int boxX = m_x + theme.padding;
            int boxY = m_y + (m_height - boxSize) / 2;

            // Draw checkbox background
            SDL_Color bgColor = m_checked ? theme.widgetPressedColor : theme.defaultWidgetColor;
            SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
            SDL_Rect boxRect{ boxX, boxY, boxSize, boxSize };
            SDL_RenderFillRect(renderer, &boxRect);

            // Draw hover highlight
            if (m_hovered)
            {
                SDL_SetRenderDrawColor(renderer, theme.widgetHoverColor.r, theme.widgetHoverColor.g,
                                       theme.widgetHoverColor.b, theme.widgetHoverColor.a);
                SDL_RenderFillRect(renderer, &boxRect);
            }

            // Draw outline using Widget helper
            {
                int oldX = m_x, oldY = m_y, oldW = m_width, oldH = m_height;
                m_x = boxX;
                m_y = boxY;
                m_width = boxSize;
                m_height = boxSize;

                drawOutline(renderer, theme);

                m_x = oldX;
                m_y = oldY;
                m_width = oldW;
                m_height = oldH;
            }

            // Draw checkmark (simple X)
            if (m_checked)
            {
                SDL_SetRenderDrawColor(renderer, theme.widgetTextColor.r, theme.widgetTextColor.g,
                                       theme.widgetTextColor.b, theme.widgetTextColor.a);
                SDL_RenderDrawLine(renderer, boxX + 3, boxY + 3, boxX + boxSize - 4, boxY + boxSize - 4);
                SDL_RenderDrawLine(renderer, boxX + 3, boxY + boxSize - 4, boxX + boxSize - 4, boxY + 3);
            }

            // Render label text
            if (m_font && !m_label.empty())
            {
                SDL_Surface* surf = TTF_RenderText_Blended(m_font, m_label.c_str(), theme.labelTextColor);
                if (surf)
                {
                    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
                    if (tex)
                    {
                        int textW, textH;
                        SDL_QueryTexture(tex, nullptr, nullptr, &textW, &textH);
                        SDL_Rect dst{ boxX + boxSize + theme.padding, m_y + (m_height - textH) / 2, textW, textH };
                        SDL_RenderCopy(renderer, tex, nullptr, &dst);
                        SDL_DestroyTexture(tex);
                    }
                    SDL_FreeSurface(surf);
                }
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
                if (m_pressed)
                {
                    setChecked(!m_checked); // emits signal automatically
                }
                m_pressed = false;
            }

            if (e.type == SDL_MOUSEBUTTONUP && !m_hovered)
                m_pressed = false;
        }

    private:
        meta::String<> m_label;
        bool m_checked = false;

        bool m_hovered = false;
        bool m_pressed = false;

        TTF_Font* m_font = nullptr;
        std::shared_ptr<Theme> m_theme;
    };
} // namespace meta::gui
