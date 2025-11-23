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

        void updateLayout(int x, int y, int w, int h, float scaleX = 1.0f, float scaleY = 1.0f) override
        {
            int currentY = y + static_cast<int>(m_padding * m_scaleY);
            int availableWidth = static_cast<int>(w - 2 * m_padding * m_scaleX);

            for (auto* widget : m_widgets)
            {
                int widgetWidth = static_cast<int>(widget->getWidth() * m_scaleX);
                int widgetHeight = static_cast<int>(widget->getHeight() * m_scaleY);
                widget->setPosition(x + static_cast<int>(m_padding * m_scaleX), currentY);
                widget->setSize(widgetWidth, widgetHeight);
                currentY += widgetHeight + static_cast<int>(m_spacing * m_scaleY);
            }

            for (auto& layout : m_childLayouts)
            {
                int layoutHeight = layout->getHeight();
                layout->updateLayout(x + static_cast<int>(m_padding * m_scaleX), currentY, availableWidth,
                                     layoutHeight);
                currentY += layoutHeight + static_cast<int>(m_spacing * m_scaleY);
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
