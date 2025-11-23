#pragma once
#include <SDL_ttf.h>
#include <algorithm>
#include <meta/base/core/String.hpp>

namespace meta::gui::utils
{
    // Calculates the width of a text string given a font
    inline int getTextWidth(TTF_Font* font, const meta::String<>& text)
    {
        if (!font || text.empty())
            return 0;

        int w = 0;
        TTF_SizeText(font, text.c_str(), &w, nullptr);
        return w;
    }

    // Splits text into lines that fit a given maximum width
    inline std::vector<meta::String<>> wrapText(TTF_Font* font, const meta::String<>& text, int maxWidth)
    {
        std::vector<meta::String<>> lines;
        if (!font || text.empty())
            return lines;

        meta::String<> currentLine;

        for (size_t i = 0; i < text.size(); ++i)
        {
            char c = text[i];
            currentLine += c;

            int w = 0;
            TTF_SizeText(font, currentLine.c_str(), &w, nullptr);

            if (w > maxWidth || c == '\n')
            {
                if (w > maxWidth && c != '\n')
                    currentLine.popBack();

                lines.push_back(currentLine);
                currentLine = meta::String<>();
                if (c != '\n')
                    currentLine += c;
            }
        }

        if (!currentLine.empty())
            lines.push_back(currentLine);

        return lines;
    }

    // Calculates total text height with line count
    inline int getTextHeight(TTF_Font* font, int lineCount)
    {
        if (!font)
            return 0;
        return TTF_FontHeight(font) * lineCount;
    }
} // namespace meta::gui::utils
