#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <memory>
#include <meta/base/core/Console.hpp>
#include <meta/gui/FontManager.hpp>
#include <meta/gui/Theme.hpp>
#include <meta/gui/widgets/Widget.hpp>
#include <vector>

namespace meta::gui
{
    class RadioButton : public Widget
    {
    public:
        RadioButton(const meta::String<>& label = "", bool checked = false,
                    const std::shared_ptr<Theme>& theme = std::make_shared<Theme>())
            : m_label(label), m_checked(checked), m_theme(theme)
        {
            loadFont();
            m_width = 120;
            m_height = m_theme->minHeight;
        }

        // Signal emitted when this button becomes checked
        Signal<bool> onChange;

        void setChecked(bool value)
        {
            if (m_checked != value)
            {
                m_checked = value;
                if (m_checked)
                {
                    // Uncheck all others in the group
                    for (auto* sibling : m_group)
                        if (sibling != this)
                            sibling->setChecked(false);
                }
                onChange.emit(m_checked);
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

            int radius = (m_height - theme.padding * 2) / 2;
            int centerX = m_x + theme.padding + radius;
            int centerY = m_y + m_height / 2;

            // Draw outer circle
            SDL_SetRenderDrawColor(renderer, theme.defaultWidgetColor.r, theme.defaultWidgetColor.g,
                                   theme.defaultWidgetColor.b, theme.defaultWidgetColor.a);
            filledCircle(renderer, centerX, centerY, radius);

            // Draw inner circle if checked
            if (m_checked)
            {
                SDL_SetRenderDrawColor(renderer, theme.widgetPressedColor.r, theme.widgetPressedColor.g,
                                       theme.widgetPressedColor.b, theme.widgetPressedColor.a);
                filledCircle(renderer, centerX, centerY, radius / 2);
            }

            // Draw label
            if (m_font && !m_label.empty())
            {
                SDL_Surface* surf = TTF_RenderText_Blended(m_font, m_label.c_str(), theme.labelTextColor);
                if (surf)
                {
                    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
                    SDL_Rect dst{ m_x + radius * 2 + theme.padding, m_y + (m_height - surf->h) / 2, surf->w, surf->h };
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
                    setChecked(true); // clicking sets this as checked
                m_pressed = false;
            }

            if (e.type == SDL_MOUSEBUTTONUP && !m_hovered)
                m_pressed = false;
        }

        // Assign this RadioButton to a group (all buttons in a group are mutually exclusive)
        void setGroup(std::vector<RadioButton*>& group)
        {
            m_group = group;
        }

    private:
        meta::String<> m_label;
        bool m_checked = false;
        bool m_hovered = false;
        bool m_pressed = false;

        TTF_Font* m_font = nullptr;
        std::shared_ptr<Theme> m_theme;
        std::vector<RadioButton*> m_group;

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

        void filledCircle(SDL_Renderer* renderer, int cx, int cy, int radius)
        {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // color handled by caller
            for (int w = 0; w < radius * 2; ++w)
                for (int h = 0; h < radius * 2; ++h)
                    if ((radius - w) * (radius - w) + (radius - h) * (radius - h) <= radius * radius)
                        SDL_RenderDrawPoint(renderer, cx + (radius - w), cy + (radius - h));
        }
    };
} // namespace meta::gui
