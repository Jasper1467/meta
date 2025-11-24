#pragma once

#include <SDL.h>
#include <memory>
#include <meta/base/core/Signal.hpp>
#include <meta/base/core/String.hpp>
#include <meta/gui/Theme.hpp>

namespace meta::gui
{
    class Theme;

    class Widget
    {
    public:
        Widget(int x = 0, int y = 0, int w = 100, int h = 30)
            : m_x(x), m_y(y), m_width(w), m_height(h), m_scaleX(1.0f), m_scaleY(1.0f)
        {
        }

        virtual ~Widget() = default;

        // Signals for common widget events
        meta::Signal<> clicked;
        meta::Signal<> hovered;
        meta::Signal<> focused;

        virtual void render(SDL_Renderer* renderer)
        {
        }

        virtual void handleEvent(const SDL_Event& e)
        {
            // Example: detect mouse click inside widget bounds
            if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                int mx = e.button.x;
                int my = e.button.y;
                if (mx >= m_x && mx <= m_x + m_width && my >= m_y && my <= m_y + m_height)
                {
                    clicked.emit();
                }
            }
            else if (e.type == SDL_MOUSEMOTION)
            {
                int mx = e.motion.x;
                int my = e.motion.y;
                if (mx >= m_x && mx <= m_x + m_width && my >= m_y && my <= m_y + m_height)
                {
                    hovered.emit();
                }
            }
            // Focus handling can be implemented in derived widgets
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

        virtual void updateLayout(int x, int y, int w, int h, float sx = 1.0f, float sy = 1.0f)
        {
            // default: widgets simply set their geometry
            setPosition(x, y);
            setSize(w, h);
            setScale(sx, sy);
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

        void setScale(float sx, float sy)
        {
            m_scaleX = sx;
            m_scaleY = sy;
        }

        int getX() const
        {
            return m_x;
        }

        int getY() const
        {
            return m_y;
        }

        bool isVisible() const
        {
            return m_visible;
        }

        void setVisible(bool v)
        {
            m_visible = v;
        }

    protected:
        void drawOutline(SDL_Renderer* renderer, const Theme& theme)
        {
            if (!theme.widgetOutlineEnable || theme.widgetOutlineSize <= 0)
                return;

            SDL_SetRenderDrawColor(renderer, theme.widgetOutlineColor.r, theme.widgetOutlineColor.g,
                                   theme.widgetOutlineColor.b, theme.widgetOutlineColor.a);

            SDL_Rect rect{ getX(), getY(), getWidth(), getHeight() };

            for (int i = 0; i < theme.widgetOutlineSize; ++i)
            {
                SDL_RenderDrawRect(renderer, &rect);
                rect.x += 1;
                rect.y += 1;
                rect.w -= 2;
                rect.h -= 2;
            }
        }

        int m_x, m_y;
        int m_width, m_height;
        float m_scaleX, m_scaleY;
        bool m_visible = true;

        std::shared_ptr<Theme> m_theme;
    };
} // namespace meta::gui
