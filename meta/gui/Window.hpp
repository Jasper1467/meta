#pragma once

#include <SDL.h>
#include <memory>
#include <meta/gui/Button.hpp>
#include <meta/gui/Layout.hpp>
#include <meta/gui/Theme.hpp>
#include <meta/gui/Widget.hpp>
#include <vector>

namespace meta::gui
{
    class Window
    {
    public:
        Window(const meta::String<>& title, int w, int h)
            : m_width(w), m_height(h), m_initialWidth(w), m_initialHeight(h), m_title(title)
        {
            SDL_Init(SDL_INIT_VIDEO);

            m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h,
                                        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

            m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

            m_theme = std::make_shared<Theme>(DEFAULT_THEME);
        }

        ~Window()
        {
            SDL_DestroyRenderer(m_renderer);
            SDL_DestroyWindow(m_window);
            SDL_Quit();
        }

        SDL_Renderer* renderer()
        {
            return m_renderer;
        }

        void setTitle(const meta::String<>& title)
        {
            m_title = title;
            if (m_window)
                SDL_SetWindowTitle(m_window, m_title.c_str());
        }

        meta::String<> getTitle() const
        {
            return m_title;
        }

        void setBackgroundColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255)
        {
            if (!m_theme)
                m_theme = std::make_shared<Theme>(DEFAULT_THEME);

            m_theme->backgroundColor = { r, g, b, a };
        }

        void addWidget(Widget* w)
        {
            m_widgets.push_back(w);
        }

        void setLayout(const std::shared_ptr<Layout>& layout)
        {
            m_layout = layout;
        }

        void setTheme(const std::shared_ptr<Theme>& theme)
        {
            m_theme = theme;
            for (auto* w : m_widgets)
                w->setTheme(theme);
            if (m_layout)
                propagateThemeRecursive(m_layout, theme);
        }

        void renderWidgets()
        {
            if (!m_layout)
                return;

            // Compute scale factors
            float scaleX = static_cast<float>(m_width) / m_initialWidth;
            float scaleY = static_cast<float>(m_height) / m_initialHeight;

            m_layout->updateLayout(0, 0, m_width, m_height, scaleX, scaleY);
            renderLayoutRecursive(m_layout, scaleX, scaleY);
        }

        void pollEvents(bool& running)
        {
            SDL_Event e;
            while (SDL_PollEvent(&e))
            {
                if (e.type == SDL_QUIT)
                    running = false;

                if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                {
                    m_width = e.window.data1;
                    m_height = e.window.data2;
                }

                if (m_layout)
                    handleLayoutEventsRecursive(m_layout, e);
            }
        }

        template <typename Func> void run(Func loop)
        {
            bool running = true;
            while (running)
            {
                SDL_GetWindowSize(m_window, &m_width, &m_height);

                SDL_SetRenderDrawColor(m_renderer, m_theme->backgroundColor.r, m_theme->backgroundColor.g,
                                       m_theme->backgroundColor.b, m_theme->backgroundColor.a);
                SDL_RenderClear(m_renderer);

                pollEvents(running);
                loop(running);
                renderWidgets();
                SDL_RenderPresent(m_renderer);
            }
        }

    private:
        void renderLayoutRecursive(const std::shared_ptr<Layout>& layout, float scaleX, float scaleY)
        {
            for (auto* w : layout->widgets())
            {
                w->setScale(scaleX, scaleY);
                w->render(m_renderer);
            }
            for (auto& l : layout->childLayouts())
                renderLayoutRecursive(l, scaleX, scaleY);
        }

        void handleLayoutEventsRecursive(const std::shared_ptr<Layout>& layout, const SDL_Event& e)
        {
            for (auto* w : layout->widgets())
                w->handleEvent(e);
            for (auto& l : layout->childLayouts())
                handleLayoutEventsRecursive(l, e);
        }

        void propagateThemeRecursive(const std::shared_ptr<Layout>& layout, const std::shared_ptr<Theme>& theme)
        {
            for (auto* w : layout->widgets())
                w->setTheme(theme);
            for (auto& l : layout->childLayouts())
                propagateThemeRecursive(l, theme);
        }

    private:
        SDL_Window* m_window = nullptr;
        SDL_Renderer* m_renderer = nullptr;
        int m_width, m_height;
        int m_initialWidth, m_initialHeight;
        meta::String<> m_title;

        std::vector<Widget*> m_widgets;
        std::shared_ptr<Layout> m_layout;
        std::shared_ptr<Theme> m_theme;
    };
} // namespace meta::gui
