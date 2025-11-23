#pragma once
#include <algorithm>
#include <meta/gui/Layout.hpp>

namespace meta::gui
{
    class VerticalLayout : public Layout
    {
    public:
        VerticalLayout(int spacing = 0, int padding = 0) : Layout(spacing, padding)
        {
        }

        void updateLayout(int x, int y, int w, int h) override
        {
            int currentY = y + m_padding;
            int availableWidth = w - 2 * m_padding;

            for (auto* widget : m_widgets)
            {
                widget->setPosition(x + m_padding, currentY);
                widget->setSize(availableWidth, widget->getHeight()); // stretch horizontally
                currentY += widget->getHeight() + m_spacing;
            }

            for (auto& layout : m_childLayouts)
            {
                layout->updateLayout(x + m_padding, currentY, availableWidth, layout->getHeight());
                currentY += layout->getHeight() + m_spacing;
            }
        }

        int getWidth() const override
        {
            int width = 0;
            for (auto* widget : m_widgets)
                width = std::max(width, widget->getWidth());
            for (auto& layout : m_childLayouts)
                width = std::max(width, layout->getWidth());
            return width + 2 * m_padding;
        }

        int getHeight() const override
        {
            int height = 0;
            for (auto* widget : m_widgets)
                height += widget->getHeight();
            for (auto& layout : m_childLayouts)
                height += layout->getHeight();
            if (!m_widgets.empty() || !m_childLayouts.empty())
                height += m_spacing * (m_widgets.size() + m_childLayouts.size() - 1);
            return height + 2 * m_padding;
        }
    };
} // namespace meta::gui
