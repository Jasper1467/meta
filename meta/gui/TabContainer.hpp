#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <memory>
#include <meta/gui/Layout.hpp>
#include <meta/gui/Tab.hpp>
#include <meta/gui/Theme.hpp>
#include <meta/gui/Widget.hpp>
#include <vector>
#include <meta/base/core/Console.hpp>

namespace meta::gui
{
    class TabContainer : public Layout
    {
    public:
        TabContainer(int x = 0, int y = 0, int w = 400, int h = 300)
            : Layout(0, 0), m_x(x), m_y(y), m_width(w), m_height(h)
        {
            if (TTF_WasInit() == 0)
            {
                if (TTF_Init() == -1)
                    meta::errorln("Failed to initialize SDL_ttf: ", TTF_GetError());
            }

            loadFont(DEFAULT_THEME);
        }

        ~TabContainer()
        {
            if (m_font)
            {
                TTF_CloseFont(m_font);
                m_font = nullptr;
            }
        }

        Signal<int> tabChanged;

        void addTab(const std::shared_ptr<Tab>& tab, const std::shared_ptr<Widget>& page)
        {
            tab->setTheme(m_theme);
            tab->setFont(m_font);

            m_tabs.push_back(tab);
            m_pages.push_back(page);

            if (m_tabs.size() == 1)
                setActiveTab(0);

            tab->onSelect.connect(
                [this, tab]()
                {
                    for (size_t i = 0; i < m_tabs.size(); ++i)
                    {
                        if (m_tabs[i] == tab)
                        {
                            setActiveTab(static_cast<int>(i));
                            break;
                        }
                    }
                });
        }

        void setActiveTab(int index)
        {
            if (index < 0 || index >= static_cast<int>(m_tabs.size()))
                return;

            m_activeTab = index;
            for (size_t i = 0; i < m_pages.size(); ++i)
                m_pages[i]->setVisible(i == static_cast<size_t>(index));

            tabChanged.emit(index);
        }

        int getActiveTab() const
        {
            return m_activeTab;
        }

        void setTheme(const std::shared_ptr<Theme>& theme) override
        {
            m_theme = theme;
            for (auto& t : m_tabs)
                t->setTheme(theme);
            for (auto& p : m_pages)
                p->setTheme(theme);

            loadFont(*theme);
        }

        void render(SDL_Renderer* renderer) override
        {
            if (!m_visible)
                return;

            const int tabHeight = 30;
            int offsetX = m_x;

            for (auto& t : m_tabs)
            {
                t->setPosition(offsetX, m_y);
                t->setSize(100, tabHeight);
                t->render(renderer);
                offsetX += t->getWidth() + 2;
            }

            if (m_activeTab >= 0 && m_activeTab < static_cast<int>(m_pages.size()))
            {
                auto& page = m_pages[m_activeTab];
                page->setPosition(m_x, m_y + tabHeight + 2);
                page->setSize(m_width, m_height - (tabHeight + 2));
                page->render(renderer);
            }
        }

        void handleEvent(const SDL_Event& e) override
        {
            if (!m_visible)
                return;

            for (auto& t : m_tabs)
                t->handleEvent(e);

            if (m_activeTab >= 0 && m_activeTab < static_cast<int>(m_pages.size()))
                m_pages[m_activeTab]->handleEvent(e);
        }

        void updateLayout(int x, int y, int w, int h, float sx = 1.0f, float sy = 1.0f) override
        {
            m_x = x;
            m_y = y;
            m_width = w;
            m_height = h;

            const int tabHeight = 30;
            int offsetX = m_x;

            for (auto& t : m_tabs)
            {
                t->setPosition(offsetX, m_y);
                t->setSize(100, tabHeight);
                offsetX += t->getWidth() + 2;
            }

            if (m_activeTab >= 0 && m_activeTab < static_cast<int>(m_pages.size()))
            {
                auto& page = m_pages[m_activeTab];
                page->setPosition(m_x, m_y + tabHeight + 2);
                page->setSize(m_width, m_height - (tabHeight + 2));
                page->updateLayout(page->getX(), page->getY(), page->getWidth(), page->getHeight(), sx, sy);
            }
        }

    private:
        void loadFont(const Theme& theme)
        {
            if (m_font)
            {
                TTF_CloseFont(m_font);
                m_font = nullptr;
            }

            if (!theme.fontPath.empty())
            {
                m_font = TTF_OpenFont(theme.fontPath.c_str(), theme.fontSize);
                if (!m_font)
                    meta::errorln("Failed to load font: ", TTF_GetError());
            }
            else
            {
                meta::errorln("No font path specified in theme.");
            }

            for (auto& t : m_tabs)
                t->setFont(m_font);
        }

    private:
        std::vector<std::shared_ptr<Tab>> m_tabs;
        std::vector<std::shared_ptr<Widget>> m_pages;
        int m_activeTab = -1;
        int m_x = 0, m_y = 0, m_width = 0, m_height = 0;
        std::shared_ptr<Theme> m_theme;
        TTF_Font* m_font = nullptr;
        bool m_visible = true;
    };
} // namespace meta::gui

