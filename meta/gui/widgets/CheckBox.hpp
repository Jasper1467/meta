#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <memory>
#include <meta/base/core/Console.hpp>
#include <meta/gui/FontManager.hpp>
#include <meta/gui/Theme.hpp>
#include <meta/gui/widgets/Widget.hpp>

namespace meta::gui
{
    class CheckBox : public Widget
    {
    public:
        CheckBox(const meta::String<>& label = "", bool checked = false,
                 const std::shared_ptr<Theme>& theme = std::make_shared<Theme>())
            : Widget(theme->minWidth, theme->minHeight, 120, theme->minHeight), m_label(label), m_checked(checked),
              m_theme(theme)
        {
            loadFont();
        }

        // Signal emitted when checkbox state changes
        Signal<bool> toggled;

        void setChecked(bool checked)
        {
            if (m_checked != checked)
            {
                m_checked = checked;
                toggled.emit(m_checked);
            }
        }

        bool isChecked() const
        {
            return m_checked;
        }

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

            int boxSize = std::max(14, m_height - 2 * theme.padding);
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

            // Draw outline
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

            // Draw checkmark
            if (m_checked)
            {
                SDL_SetRenderDrawColor(renderer, theme.widgetTextColor.r, theme.widgetTextColor.g,
                                       theme.widgetTextColor.b, theme.widgetTextColor.a);
                SDL_RenderDrawLine(renderer, boxX + 3, boxY + 3, boxX + boxSize - 4, boxY + boxSize - 4);
                SDL_RenderDrawLine(renderer, boxX + 3, boxY + boxSize - 4, boxX + boxSize - 4, boxY + 3);
            }

            // Draw label
            if (m_font && !m_label.empty())
            {
                SDL_Surface* surface = TTF_RenderText_Blended(m_font, m_label.c_str(), theme.labelTextColor);
                if (surface)
                {
                    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                    int textW = surface->w;
                    int textH = surface->h;
                    SDL_Rect dst{ boxX + boxSize + theme.padding, m_y + (m_height - textH) / 2, textW, textH };
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
                    setChecked(!m_checked); // emits signal
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
            int boxSize = std::max(14, m_height - 2 * theme.padding);
            return std::max(m_width, boxSize + theme.padding + textWidth + theme.padding);
        }

        int getHeight() const override
        {
            const Theme& theme = m_theme ? *m_theme : DEFAULT_THEME;
            int textHeight = m_font ? TTF_FontHeight(m_font) : 0;
            return std::max(m_height, textHeight + 2 * theme.padding);
        }

    private:
        meta::String<> m_label;
        bool m_checked = false;

        bool m_hovered = false;
        bool m_pressed = false;

        TTF_Font* m_font = nullptr;
        std::shared_ptr<Theme> m_theme;

        void loadFont()
        {
            if (m_theme && !m_theme->fontPath.empty())
            {
                m_font = FontManager::instance().loadFont(m_theme->fontPath.c_str(), m_theme->fontSize);
                if (!m_font)
                    meta::errorln("CheckBox: Failed to load font from FontManager!");
            }
            else
            {
                meta::errorln("CheckBox: No font path specified in theme.");
            }
        }
    };
} // namespace meta::gui
