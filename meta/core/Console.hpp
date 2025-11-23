#pragma once

#include <iostream>
#include <meta/core/Platform.hpp>
#include <meta/core/Format.hpp>
#include <meta/core/String.hpp>

namespace meta
{
    namespace internal
    {
        // --- No-arg version (avoids constructing empty meta::String) ---
        template <typename Stream>
        META_INLINE void consoleWriteTo(Stream&) {}

        template <typename Stream, typename... Args>
        META_INLINE void consoleWriteTo(Stream& os, Args&&... args)
        {
            meta::String<> out = meta::format(std::forward<Args>(args)...);
            os << out.c_str();
        }

        template <typename Stream, typename... Args>
        META_INLINE void consoleWriteLine(Stream& os, Args&&... args)
        {
            consoleWriteTo(os, std::forward<Args>(args)...);
            os << '\n';
        }
    }

    // --- Standard print ---
    template <typename... Args>
    META_INLINE void print(Args&&... args)
    {
        internal::consoleWriteTo(std::cout, std::forward<Args>(args)...);
    }

    template <typename... Args>
    META_INLINE void println(Args&&... args)
    {
        internal::consoleWriteLine(std::cout, std::forward<Args>(args)...);
    }

    // --- Error output ---
    template <typename... Args>
    META_INLINE void error(Args&&... args)
    {
        internal::consoleWriteTo(std::cerr, std::forward<Args>(args)...);
    }

    template <typename... Args>
    META_INLINE void errorln(Args&&... args)
    {
        internal::consoleWriteLine(std::cerr, std::forward<Args>(args)...);
    }

    // --- Debug output (only compiled in debug builds) ---
    template <typename... Args>
    META_INLINE void debug(Args&&... args)
    {
#ifdef META_DEBUG
        internal::consoleWriteTo(std::cout, std::forward<Args>(args)...);
#endif
    }

    template <typename... Args>
    META_INLINE void debugln(Args&&... args)
    {
#ifdef META_DEBUG
        internal::consoleWriteLine(std::cout, std::forward<Args>(args)...);
#endif
    }

} // namespace meta
