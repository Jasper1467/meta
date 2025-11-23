#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <meta/base/core/Console.hpp>
#include <meta/base/core/String.hpp>
#include <meta/gui/TextUtils.hpp>
#include <meta/gui/Theme.hpp>
#include <meta/gui/Widget.hpp>

namespace meta::gui
{
    class TextBox : public Widget
    {
    public:
        TextBox(const meta::String<>& label = "", const meta::String<>& initialText = "")
            : Widget(DEFAULT_THEME.minWidth, DEFAULT_THEME.minHeight, DEFAULT_THEME.minWidth, DEFAULT_THEME.minHeight),
              m_label(label), m_text(initialText)
        {
            if (TTF_WasInit() == 0)
                TTF_Init();

            m_font = TTF_OpenFont(DEFAULT_THEME.fontPath.c_str(), DEFAULT_THEME.fontSize);
            if (!m_font)
                meta::errorln("Failed to load font: ", TTF_GetError());
        }

        // Signal emitted whenever the text changes
        meta::Signal<const meta::String<>&> textChanged;

        void setTheme(const std::shared_ptr<Theme>& theme) override
        {
            m_theme = theme;
        }

        void setText(const meta::String<>& text)
        {
            if (m_text != text)
            {
                m_text = text;
                textChanged.emit(m_text);
            }
        }

        const meta::String<>& getText() const
        {
            return m_text;
        }

        void render(SDL_Renderer* renderer) override
        {
            if (!m_visible || !m_font)
                return;

            const Theme& theme = m_theme ? *m_theme : DEFAULT_THEME;
            int labelHeight = 0;

            // Draw label
            if (!m_label.empty())
            {
                SDL_Surface* surface = TTF_RenderText_Blended(m_font, m_label.c_str(), theme.labelTextColor);
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

            // Draw text box background
            SDL_Rect rect{ m_x, m_y + labelHeight, m_width, m_height };
            drawRoundedRect(renderer, rect, theme.defaultWidgetColor, theme.borderRadius);

            // Draw wrapped text
            int yOffset = m_y + labelHeight + theme.padding;
            auto lines = meta::gui::utils::wrapText(m_font, m_text, m_width - 2 * theme.padding);

            for (const auto& line : lines)
            {
                SDL_Surface* lineSurface = TTF_RenderText_Blended(m_font, line.c_str(), theme.widgetTextColor);
                if (lineSurface)
                {
                    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, lineSurface);
                    SDL_Rect dst{ m_x + theme.padding, yOffset, lineSurface->w, lineSurface->h };
                    SDL_RenderCopy(renderer, texture, nullptr, &dst);
                    SDL_DestroyTexture(texture);
                    SDL_FreeSurface(lineSurface);
                }
                yOffset += TTF_FontHeight(m_font) + theme.textSpacing;
            }

            drawOutline(renderer, theme);
        }

        void handleEvent(const SDL_Event& e) override
        {
            if (!m_visible)
                return;

            if (e.type == SDL_TEXTINPUT)
            {
                m_text += e.text.text;
                textChanged.emit(m_text);
            }
            else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_BACKSPACE && !m_text.empty())
            {
                m_text.popBack();
                textChanged.emit(m_text);
            }
        }

        int getWidth() const override
        {
            int boxWidth = m_width;
            const Theme& theme = m_theme ? *m_theme : DEFAULT_THEME;

            if (m_font)
            {
                if (!m_label.empty())
                    boxWidth = std::max(boxWidth, meta::gui::utils::getTextWidth(m_font, m_label) + 2 * theme.padding);

                auto lines = meta::gui::utils::wrapText(m_font, m_text, 10000); // large width
                int maxLineW = 0;
                for (const auto& line : lines)
                    maxLineW = std::max(maxLineW, meta::gui::utils::getTextWidth(m_font, line));

                boxWidth = std::max(boxWidth, maxLineW + 2 * theme.padding);
            }

            return std::max(boxWidth, theme.minWidth);
        }

        int getHeight() const override
        {
            const Theme& theme = m_theme ? *m_theme : DEFAULT_THEME;
            int labelHeight = (!m_label.empty() && m_font) ? TTF_FontHeight(m_font) + theme.textSpacing : 0;
            int lineHeight = m_font ? TTF_FontHeight(m_font) : m_height;

            auto lines = meta::gui::utils::wrapText(m_font, m_text, m_width - 2 * theme.padding);
            int totalLines = static_cast<int>(lines.size());
            if (totalLines == 0)
                totalLines = 1;

            return labelHeight + totalLines * (lineHeight + theme.textSpacing) + 2 * theme.padding;
        }

    private:
        meta::String<> m_label;
        meta::String<> m_text;
        TTF_Font* m_font = nullptr;
        std::shared_ptr<Theme> m_theme;

        void drawRoundedRect(SDL_Renderer* renderer, const SDL_Rect& rect, const SDL_Color& color, int radius)
        {
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            SDL_RenderFillRect(renderer, &rect); // placeholder for rounded rect
        }
    };
} // namespace meta::gui
