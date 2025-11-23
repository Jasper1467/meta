#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <functional>
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
            m_theme = std::make_shared<Theme>(DEFAULT_THEME);
            initFont(m_theme->fontPath, m_theme->fontSize);
            updateSize();
        }

        void setText(const meta::String<>& text)
        {
            m_text = text;
            updateSize();
        }

        void setOnClick(std::function<void()> callback)
        {
            m_onClick = callback;
        }

        void setTheme(const std::shared_ptr<Theme>& theme) override
        {
            m_theme = theme ? theme : std::make_shared<Theme>(DEFAULT_THEME);
            initFont(m_theme->fontPath, m_theme->fontSize);
            updateSize();
        }

        void render(SDL_Renderer* renderer) override
        {
            if (!m_visible || !m_font || m_text.empty())
                return;

            const Theme& theme = *m_theme;
            SDL_Color color = m_hovered ? theme.widgetHoverColor : theme.widgetTextColor;

            SDL_Surface* surface = TTF_RenderText_Blended(m_font, m_text.c_str(), color);
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
                if (m_pressed && m_onClick)
                    m_onClick();
                m_pressed = false;
            }

            if (e.type == SDL_MOUSEBUTTONUP && !m_hovered)
                m_pressed = false;
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
                setSize(w, h);
        }

    private:
        meta::String<> m_text;
        TTF_Font* m_font = nullptr;
        std::shared_ptr<Theme> m_theme;

        bool m_hovered = false;
        bool m_pressed = false;
        std::function<void()> m_onClick;
    };
} // namespace meta::gui
