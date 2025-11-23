#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <memory>
#include <meta/base/core/Console.hpp>
#include <meta/base/core/String.hpp>
#include <meta/gui/Theme.hpp>
#include <meta/gui/Widget.hpp>

namespace meta::gui
{
    class Label : public Widget
    {
    public:
        Label(const meta::String<>& text = "") : Widget(0, 0, 0, 0), m_text(text)
        {
            initFont(DEFAULT_THEME.fontPath, DEFAULT_THEME.fontSize);
            updateSize();
        }

        void setText(const meta::String<>& text)
        {
            m_text = text;
            updateSize();
        }

        void setTheme(const std::shared_ptr<Theme>& theme) override
        {
            m_theme = theme;
            int fontSize = theme ? theme->fontSize : DEFAULT_THEME.fontSize;
            const auto& fontPath = theme ? theme->fontPath : DEFAULT_THEME.fontPath;
            initFont(fontPath, fontSize);
            updateSize();
        }

        void render(SDL_Renderer* renderer) override
        {
            if (!m_visible || !m_font || m_text.empty())
                return;

            const Theme& theme = m_theme ? *m_theme : DEFAULT_THEME;
            SDL_Color textColor = theme.widgetTextColor;

            SDL_Surface* surface = TTF_RenderText_Blended(m_font, m_text.c_str(), textColor);
            if (!surface)
                return;

            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            if (!texture)
            {
                SDL_FreeSurface(surface);
                return;
            }

            SDL_Rect dst{ m_x, m_y, surface->w, surface->h };
            SDL_RenderCopy(renderer, texture, nullptr, &dst);

            SDL_DestroyTexture(texture);
            SDL_FreeSurface(surface);
        }

    private:
        void initFont(const meta::String<>& path, int size)
        {
            if (TTF_WasInit() == 0)
                TTF_Init();

            if (m_font)
                TTF_CloseFont(m_font);

            m_font = TTF_OpenFont(path.c_str(), size);
            if (!m_font)
                meta::errorln("Failed to load font: ", TTF_GetError());
        }

        void updateSize()
        {
            if (!m_font || m_text.empty())
                return;

            int w = 0, h = 0;
            if (TTF_SizeText(m_font, m_text.c_str(), &w, &h) == 0)
            {
                setSize(w, h);
            }
        }

    private:
        meta::String<> m_text;
        TTF_Font* m_font = nullptr;
        std::shared_ptr<Theme> m_theme;
    };
} // namespace meta::gui
