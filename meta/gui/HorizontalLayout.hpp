#pragma once
#include <algorithm>
#include <meta/gui/Layout.hpp>

namespace meta::gui
{
    class HorizontalLayout : public Layout
    {
    public:
        HorizontalLayout(int spacing = 0, int padding = 0) : Layout(spacing, padding)
        {
        }

        void updateLayout(int x, int y, int w, int h) override
        {
            int currentX = x + m_padding;
            int availableHeight = h - 2 * m_padding;

            for (auto* widget : m_widgets)
            {
                widget->setPosition(currentX, y + m_padding);
                widget->setSize(widget->getWidth(), availableHeight); // stretch vertically
                currentX += widget->getWidth() + m_spacing;
            }

            for (auto& layout : m_childLayouts)
            {
                layout->updateLayout(currentX, y + m_padding, layout->getWidth(), availableHeight);
                currentX += layout->getWidth() + m_spacing;
            }
        }

        int getWidth() const override
        {
            int width = 0;
            for (auto* widget : m_widgets)
                width += widget->getWidth();
            for (auto& layout : m_childLayouts)
                width += layout->getWidth();
            if (!m_widgets.empty() || !m_childLayouts.empty())
                width += m_spacing * (m_widgets.size() + m_childLayouts.size() - 1);
            return width + 2 * m_padding;
        }

        int getHeight() const override
        {
            int height = 0;
            for (auto* widget : m_widgets)
                height = std::max(height, widget->getHeight());
            for (auto& layout : m_childLayouts)
                height = std::max(height, layout->getHeight());
            return height + 2 * m_padding;
        }
    };
} // namespace meta::gui
