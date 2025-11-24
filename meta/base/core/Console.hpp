#pragma once

#include <iostream>
#include <magic_enum/magic_enum.hpp>
#include <meta/base/core/Format.hpp>
#include <meta/base/core/Platform.hpp>
#include <meta/base/core/String.hpp>

namespace meta
{
    // --- Console color enumeration ---
    enum class ConsoleColor
    {
        Default,
        Black,
        Red,
        Green,
        Yellow,
        Blue,
        Magenta,
        Cyan,
        White
    };

    namespace internal
    {
        // --- Convert color to ANSI escape code ---
        inline const char* colorToAnsi(ConsoleColor color)
        {
            switch (color)
            {
            case ConsoleColor::Black:
                return "\033[30m";
            case ConsoleColor::Red:
                return "\033[31m";
            case ConsoleColor::Green:
                return "\033[32m";
            case ConsoleColor::Yellow:
                return "\033[33m";
            case ConsoleColor::Blue:
                return "\033[34m";
            case ConsoleColor::Magenta:
                return "\033[35m";
            case ConsoleColor::Cyan:
                return "\033[36m";
            case ConsoleColor::White:
                return "\033[37m";
            default:
                return "\033[0m"; // Reset
            }
        }

        // --- No-arg version (avoids constructing empty meta::String) ---
        template <typename Stream> META_INLINE void consoleWriteTo(Stream&)
        {
        }

        template <typename Stream, typename... Args> META_INLINE void consoleWriteTo(Stream& os, Args&&... args)
        {
            meta::String<> out = meta::format(std::forward<Args>(args)...);
            os << out.c_str();
        }

        template <typename Stream, typename... Args> META_INLINE void consoleWriteLine(Stream& os, Args&&... args)
        {
            consoleWriteTo(os, std::forward<Args>(args)...);
            os << '\n';
        }

        template <typename Stream, typename... Args>
        META_INLINE void consoleWriteColor(Stream& os, ConsoleColor color, Args&&... args)
        {
            os << colorToAnsi(color);
            consoleWriteTo(os, std::forward<Args>(args)...);
            os << colorToAnsi(ConsoleColor::Default);
        }

        template <typename Stream, typename... Args>
        META_INLINE void consoleWriteLineColor(Stream& os, ConsoleColor color, Args&&... args)
        {
            os << colorToAnsi(color);
            consoleWriteLine(os, std::forward<Args>(args)...);
            os << colorToAnsi(ConsoleColor::Default);
        }
    } // namespace internal

    // --- Standard print ---
    template <typename... Args> META_INLINE void print(Args&&... args)
    {
        internal::consoleWriteTo(std::cout, std::forward<Args>(args)...);
    }

    template <typename... Args> META_INLINE void println(Args&&... args)
    {
        internal::consoleWriteLine(std::cout, std::forward<Args>(args)...);
    }

    template <typename... Args> META_INLINE void printColor(ConsoleColor color, Args&&... args)
    {
        internal::consoleWriteColor(std::cout, color, std::forward<Args>(args)...);
    }

    template <typename... Args> META_INLINE void printlnColor(ConsoleColor color, Args&&... args)
    {
        internal::consoleWriteLineColor(std::cout, color, std::forward<Args>(args)...);
    }

    // --- Error output ---
    template <typename... Args> META_INLINE void error(Args&&... args)
    {
        internal::consoleWriteTo(std::cerr, std::forward<Args>(args)...);
    }

    template <typename... Args> META_INLINE void errorln(Args&&... args)
    {
        internal::consoleWriteLine(std::cerr, std::forward<Args>(args)...);
    }

    template <typename... Args> META_INLINE void errorColor(ConsoleColor color, Args&&... args)
    {
        internal::consoleWriteColor(std::cerr, color, std::forward<Args>(args)...);
    }

    template <typename... Args> META_INLINE void errorlnColor(ConsoleColor color, Args&&... args)
    {
        internal::consoleWriteLineColor(std::cerr, color, std::forward<Args>(args)...);
    }

    // --- Debug output (only compiled in debug builds) ---
    template <typename... Args> META_INLINE void debug(Args&&... args)
    {
#ifdef META_DEBUG
        internal::consoleWriteTo(std::cout, std::forward<Args>(args)...);
#endif
    }

    template <typename... Args> META_INLINE void debugln(Args&&... args)
    {
#ifdef META_DEBUG
        internal::consoleWriteLine(std::cout, std::forward<Args>(args)...);
#endif
    }

    template <typename... Args> META_INLINE void debugColor(ConsoleColor color, Args&&... args)
    {
#ifdef META_DEBUG
        internal::consoleWriteColor(std::cout, color, std::forward<Args>(args)...);
#endif
    }

    template <typename... Args> META_INLINE void debuglnColor(ConsoleColor color, Args&&... args)
    {
#ifdef META_DEBUG
        internal::consoleWriteLineColor(std::cout, color, std::forward<Args>(args)...);
#endif
    }
} // namespace meta
