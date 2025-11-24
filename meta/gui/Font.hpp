#pragma once

#include <SDL_ttf.h>
#include <meta/base/core/String.hpp>
#include <memory>

namespace meta::gui
{
    class Font
    {
    public:
        Font(const meta::String<>& path, int size)
            : m_path(path), m_size(size)
        {
            if (TTF_Init() == -1)
            {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "TTF_Init failed: %s", TTF_GetError());
                return;
            }

            m_font = TTF_OpenFont(path.c_str(), size);
            if (!m_font)
            {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load font: %s", path.c_str());
            }
        }

        ~Font()
        {
            if (m_font)
                TTF_CloseFont(m_font);

            TTF_Quit();
        }

        TTF_Font* raw()
        {
            return m_font;
        }

        meta::String<> getPath() const
        {
            return m_path;
        }

        int getSize() const
        {
            return m_size;
        }

        bool isValid() const
        {
            return m_font != nullptr;
        }

    private:
        TTF_Font* m_font = nullptr;
        meta::String<> m_path;
        int m_size = 0;
    };
} // namespace meta::gui

