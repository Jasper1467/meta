#pragma once
#include <SDL_ttf.h>
#include <unordered_map>
#include <string>
#include <meta/base/core/Console.hpp>

namespace meta::gui
{
    class FontManager
    {
    public:
        static FontManager& instance()
        {
            static FontManager fm;
            return fm;
        }

        // Load font by path and size, cached to avoid reopening multiple times
        TTF_Font* loadFont(const std::string& path, int size)
        {
            // Key: path + size
            std::string key = path + ":" + std::to_string(size);
            if (m_fonts.count(key))
                return m_fonts[key];

            TTF_Font* font = TTF_OpenFont(path.c_str(), size);
            if (!font)
                meta::errorln("Failed to load font: ", TTF_GetError());
            else
                m_fonts[key] = font;

            return font;
        }

        // Close all fonts on shutdown
        ~FontManager()
        {
            for (auto& [k, font] : m_fonts)
                TTF_CloseFont(font);
            m_fonts.clear();

            if (TTF_WasInit())
                TTF_Quit();
        }

    private:
        FontManager()
        {
            if (TTF_WasInit() == 0 && TTF_Init() == -1)
                meta::errorln("Failed to initialize SDL_ttf: ", TTF_GetError());
        }

        FontManager(const FontManager&) = delete;
        FontManager& operator=(const FontManager&) = delete;

        std::unordered_map<std::string, TTF_Font*> m_fonts;
    };
} // namespace meta::gui
