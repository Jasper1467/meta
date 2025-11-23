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

            for (auto* wgt : m_widgets)
            {
                int widgetWidth = wgt->getWidth();
                int widgetHeight = wgt->getHeight();
                wgt->setPosition(x + m_padding, currentY);
                wgt->setSize(widgetWidth, widgetHeight);
                currentY += widgetHeight + m_spacing;
            }

            for (auto& child : m_childLayouts)
            {
                int childWidth = child->getWidth();
                int childHeight = child->getHeight();
                child->updateLayout(x + m_padding, currentY, childWidth, childHeight);
                currentY += childHeight + m_spacing;
            }
        }

        int getWidth() const override
        {
            int width = 0;
            for (auto* wgt : m_widgets)
                width = std::max(width, wgt->getWidth());
            for (auto& child : m_childLayouts)
                width = std::max(width, child->getWidth());

            return width + 2 * m_padding;
        }

        int getHeight() const override
        {
            int height = 0;
            for (auto* wgt : m_widgets)
                height += wgt->getHeight();
            for (auto& child : m_childLayouts)
                height += child->getHeight();

            if (!m_widgets.empty() || !m_childLayouts.empty())
                height += m_spacing * (m_widgets.size() + m_childLayouts.size() - 1);

            return height + 2 * m_padding;
        }
    };
} // namespace meta::gui
