#pragma once
#include <SDL_ttf.h>
#include <meta/base/core/Console.hpp>
#include <string>
#include <unordered_map>

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
            auto key = path + "#" + std::to_string(size);
            auto it = m_fonts.find(key);
            if (it != m_fonts.end())
                return it->second;

            if (path.empty())
            {
                meta::errorln("FontManager: font path is empty, cannot load font.");
                return nullptr;
            }

            TTF_Font* font = TTF_OpenFont(path.c_str(), size);
            if (!font)
            {
                meta::errorln("FontManager: failed to load font: ", TTF_GetError());
                return nullptr;
            }

            m_fonts[key] = font;
            return font;
        }

        // Close all fonts on shutdown
        ~FontManager()
        {
            for (auto& pair : m_fonts)
            {
                if (pair.second)
                    TTF_CloseFont(pair.second);
            }

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
