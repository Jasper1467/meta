#pragma once
#include <SDL.h>
#include <meta/base/core/Platform.hpp>
#include <meta/base/core/String.hpp>

namespace meta::gui
{
    struct Theme
    {
        // Window / background colors
        SDL_Color backgroundColor{ 240, 240, 240, 255 }; // Light gray background

        // widget colors
        SDL_Color defaultWidgetColor{ 225, 225, 225, 255 }; // Slightly darker than background
        SDL_Color widgetHoverColor{ 200, 200, 255, 255 };   // Subtle blue highlight
        SDL_Color widgetPressedColor{ 180, 180, 240, 255 }; // Slightly darker blue when pressed
        SDL_Color widgetTextColor{ 50, 50, 50, 255 };       // Dark text for contrast

        int fontSize = 14;     // Slightly smaller default font
        int padding{ 6 };      // Padding inside widgets
        int spacing{ 6 };      // Space between widgets
        int borderRadius{ 6 }; // Rounded corners for buttons/widgets

        meta::String<> fontPath;

        Theme()
        {
#if defined(META_PLATFORM_LINUX)
            fontPath = "/usr/share/fonts/TTF/DejaVuSans.ttf";
#elif defined(META_PLATFORM_WINDOWS)
            fontPath = "C:\\Windows\\Fonts\\Arial.ttf";
#elif defined(META_PLATFORM_MAC)
            fontPath = "/System/Library/Fonts/SFNS.ttf"; // macOS system font
#else
            fontPath = ""; // fallback
#endif
        }
    };

    inline const Theme DEFAULT_THEME{};
} // namespace meta::gui
