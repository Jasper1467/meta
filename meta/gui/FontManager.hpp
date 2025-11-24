#pragma once
#include <SDL_ttf.h>
#include <meta/base/core/Console.hpp>
#include <meta/base/filesystem/Path.hpp>
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

        // Load font by Path and size, cached to avoid reopening multiple times
        TTF_Font* loadFont(const Path& path, int size)
        {
            if (path.empty())
            {
                meta::errorln("FontManager: font path is empty, cannot load font.");
                return nullptr;
            }

            std::string key = path.toString() + "#" + std::to_string(size);
            auto it = m_fonts.find(key);
            if (it != m_fonts.end())
                return it->second;

            TTF_Font* font = TTF_OpenFont(path.c_str(), size);
            if (!font)
            {
                meta::errorln("FontManager: failed to load font '", path, "': ", TTF_GetError());
                return nullptr;
            }

            m_fonts[key] = font;
            return font;
        }

        // Close all fonts safely on shutdown
        ~FontManager()
        {
            for (auto& [key, font] : m_fonts)
            {
                if (font)
                    TTF_CloseFont(font);
            }
            m_fonts.clear();

            if (TTF_WasInit())
                TTF_Quit();
        }

    private:
        FontManager()
        {
            if (TTF_WasInit() == 0 && TTF_Init() == -1)
                meta::errorln("FontManager: Failed to initialize SDL_ttf: ", TTF_GetError());
        }

        FontManager(const FontManager&) = delete;
        FontManager& operator=(const FontManager&) = delete;

        std::unordered_map<std::string, TTF_Font*> m_fonts;
    };
} // namespace meta::gui
