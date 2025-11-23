#pragma once
#include <SDL.h>
#include <memory>
#include <meta/base/core/String.hpp>

namespace meta::gui
{
    class Theme;

    class Widget
    {
    public:
        Widget(int x = 0, int y = 0, int w = 100, int h = 30) : m_x(x), m_y(y), m_width(w), m_height(h)
        {
        }

        virtual ~Widget() = default;

        virtual void render(SDL_Renderer* renderer)
        {
        }

        virtual void handleEvent(const SDL_Event& e)
        {
        }

        virtual void setTheme(const std::shared_ptr<Theme>& theme)
        {
            m_theme = theme;
        }

        virtual int getWidth() const
        {
            return m_width;
        }

        virtual int getHeight() const
        {
            return m_height;
        }

        int getX() const
        {
            return m_x;
        }

        int getY() const
        {
            return m_y;
        }

        void setPosition(int x, int y)
        {
            m_x = x;
            m_y = y;
        }
        void setSize(int w, int h)
        {
            m_width = w;
            m_height = h;
        }

        int m_x, m_y;
        bool m_visible = true;

    protected:
        int m_width, m_height;
        std::shared_ptr<Theme> m_theme;
    };
} // namespace meta::gui
