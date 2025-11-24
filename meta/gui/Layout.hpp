#pragma once
#include <memory>
#include <meta/gui/Theme.hpp>
#include <meta/gui/Widget.hpp>
#include <vector>

namespace meta::gui
{
    class Layout : public Widget
    {
    public:
        Layout(int spacing = 0, int padding = 0)
            : Widget(), m_spacing(spacing), m_padding(padding), m_scaleX(1.0f), m_scaleY(1.0f)
        {
        }

        virtual ~Layout() = default;

        void addWidget(Widget* w)
        {
            m_widgets.push_back(w);
        }

        void addLayout(std::shared_ptr<Layout> l)
        {
            m_childLayouts.push_back(l);
        }

        const std::vector<Widget*>& widgets() const
        {
            return m_widgets;
        }

        const std::vector<std::shared_ptr<Layout>>& childLayouts() const
        {
            return m_childLayouts;
        }

        void setSpacing(int s)
        {
            m_spacing = s;
        }
        void setPadding(int p)
        {
            m_padding = p;
        }

        // Scaling
        void setScale(float sx, float sy)
        {
            m_scaleX = sx;
            m_scaleY = sy;

            for (auto* w : m_widgets)
                w->setScale(sx, sy);
            for (auto& l : m_childLayouts)
                l->setScale(sx, sy);
        }

        // Allow applying a theme to the layout and propagate to children
        // Subclasses may override to react differently.
        virtual void setTheme(const std::shared_ptr<Theme>& theme)
        {
            // propagate to widgets and child layouts
            for (auto* w : m_widgets)
                w->setTheme(theme);
            for (auto& l : m_childLayouts)
                l->setTheme(theme);

            m_theme = theme;
        }

        virtual void updateLayout(int x, int y, int w, int h, float scaleX = 1.0f, float scaleY = 1.0f) = 0;

        virtual int getWidth() const
        {
            int width = 0;
            for (auto* w : m_widgets)
                width = std::max(width, w->getWidth());
            for (auto& l : m_childLayouts)
                width = std::max(width, l->getWidth());
            return static_cast<int>((width + 2 * m_padding) * m_scaleX);
        }

        virtual int getHeight() const
        {
            int height = 0;
            for (auto* w : m_widgets)
                height += w->getHeight();
            for (auto& l : m_childLayouts)
                height += l->getHeight();
            if (!m_widgets.empty() || !m_childLayouts.empty())
                height += m_spacing * (m_widgets.size() + m_childLayouts.size() - 1);
            return static_cast<int>((height + 2 * m_padding) * m_scaleY);
        }

        virtual void render(SDL_Renderer* renderer) override
        {
            for (auto* w : m_widgets)
                w->render(renderer);

            for (auto& l : m_childLayouts)
                l->render(renderer);
        }

        virtual void handleEvent(const SDL_Event& e) override
        {
            for (auto* w : m_widgets)
                w->handleEvent(e);

            for (auto& l : m_childLayouts)
                l->handleEvent(e);
        }

    protected:
        std::vector<Widget*> m_widgets;
        std::vector<std::shared_ptr<Layout>> m_childLayouts;
        int m_spacing;
        int m_padding;
        float m_scaleX, m_scaleY;

        std::shared_ptr<Theme> m_theme;
    };
} // namespace meta::gui
