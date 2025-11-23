#pragma once
#include <memory>
#include <meta/gui/Widget.hpp>
#include <vector>

namespace meta::gui
{
    class Layout
    {
    public:
        Layout(int spacing = 0, int padding = 0) : m_spacing(spacing), m_padding(padding)
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

        virtual void updateLayout(int x, int y, int w, int h) = 0;

        virtual int getWidth() const
        {
            int width = 0;
            for (auto* w : m_widgets)
                width = std::max(width, w->getWidth());
            for (auto& l : m_childLayouts)
                width = std::max(width, l->getWidth());
            return width + 2 * m_padding;
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
            return height + 2 * m_padding;
        }

    protected:
        std::vector<Widget*> m_widgets;
        std::vector<std::shared_ptr<Layout>> m_childLayouts;
        int m_spacing;
        int m_padding;
    };
} // namespace meta::gui
