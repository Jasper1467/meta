#pragma once
#include <algorithm>
#include <meta/gui/layouts/Layout.hpp>

namespace meta::gui
{
    class HorizontalLayout : public Layout
    {
    public:
        HorizontalLayout(int spacing = 0, int padding = 0) : Layout(spacing, padding)
        {
        }

        void updateLayout(int x, int y, int w, int h, float scaleX = 1.0f, float scaleY = 1.0f) override
        {
            int currentX = x + static_cast<int>(m_padding * m_scaleX);
            int availableHeight = static_cast<int>(h - 2 * m_padding * m_scaleY);

            for (auto* widget : m_widgets)
            {
                int widgetWidth = static_cast<int>(widget->getWidth() * m_scaleX);
                int widgetHeight = static_cast<int>(widget->getHeight() * m_scaleY);
                widget->setPosition(currentX, y + static_cast<int>(m_padding * m_scaleY));
                widget->setSize(widgetWidth, widgetHeight);
                currentX += widgetWidth + static_cast<int>(m_spacing * m_scaleX);
            }

            for (auto& layout : m_childLayouts)
            {
                int layoutWidth = layout->getWidth();
                layout->updateLayout(currentX, y + static_cast<int>(m_padding * m_scaleY), layoutWidth,
                                     availableHeight);
                currentX += layoutWidth + static_cast<int>(m_spacing * m_scaleX);
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
