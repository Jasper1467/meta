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

        // Widget colors
        SDL_Color defaultWidgetColor{ 225, 225, 225, 255 }; // Slightly darker than background
        SDL_Color widgetHoverColor{ 200, 200, 255, 255 };   // Subtle blue highlight
        SDL_Color widgetPressedColor{ 180, 180, 240, 255 }; // Slightly darker blue when pressed
        SDL_Color widgetTextColor{ 50, 50, 50, 255 };       // Dark text for contrast
        SDL_Color widgetOutlineColor{ 0, 0, 0, 255 };

        // Text-specific colors (for labels, textboxes, sliders, etc.)
        SDL_Color labelTextColor{ 40, 40, 40, 255 };          // Slightly darker than default text
        SDL_Color placeholderTextColor{ 150, 150, 150, 255 }; // For empty textboxes
        SDL_Color selectionColor{ 100, 150, 255, 100 };       // For selected/highlighted text

        // Toggle (Switch) specific theme options
        SDL_Color toggleOnColor{ 100, 200, 255, 255 };      // Bright blue track when active
        SDL_Color toggleOffColor{ 180, 180, 180, 255 };     // Gray track when inactive
        SDL_Color toggleKnobColor{ 255, 255, 255, 255 };    // White knob
        SDL_Color toggleOutlineColor{ 120, 120, 120, 255 }; // Optional outline
        int toggleOutlineSize{ 1 };                         // Outline thickness
        int toggleTrackHeight{ 20 };                        // Default height of switch track
        int toggleKnobMargin{ 2 };                          // Space inside track for knob

        int fontSize = 15;                // Default font size
        int textSpacing{ 4 };             // Spacing between lines of text
        int padding{ 8 };                 // Padding inside widgets
        int spacing{ 10 };                // Space between widgets
        int borderRadius{ 8 };            // Rounded corners for buttons/widgets
        int minWidth{ 80 };               // Minimum width for widgets
        int minHeight{ 24 };              // Minimum height for widgets
        int widgetOutlineSize{ 1 };       // Outline thickness for widgets
        bool widgetOutlineEnable{ false }; // enable/disable outlines globally

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
