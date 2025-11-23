#pragma once

#include <cstddef>
#include <iostream>
#include <meta/base/core/String.hpp>

namespace meta
{
    class Path
    {
    public:
        META_INLINE Path() = default;

        META_INLINE Path(const String<>& str) : m_path(normalizeSeparators(str))
        {
        }
        META_INLINE Path(String<>&& str) : m_path(normalizeSeparators(std::move(str)))
        {
        }
        META_INLINE Path(const char* s) : m_path(normalizeSeparators(String<>(s)))
        {
        }
        META_INLINE Path(std::string_view sv) : m_path(normalizeSeparators(String<>(sv)))
        {
        }

        META_INLINE Path(const Path& other) : m_path(other.m_path)
        {
        }
        META_INLINE Path(Path&& other) noexcept : m_path(std::move(other.m_path))
        {
        }

        META_INLINE Path& operator=(const Path& other)
        {
            if (this != &other)
                m_path = other.m_path;
            return *this;
        }

        META_INLINE Path& operator=(Path&& other) noexcept
        {
            if (this != &other)
                m_path = std::move(other.m_path);
            return *this;
        }

        META_NODISCARD META_INLINE const String<>& str() const noexcept
        {
            return m_path;
        }
        META_NODISCARD META_INLINE const char* c_str() const noexcept
        {
            return m_path.c_str();
        }
        META_NODISCARD META_INLINE size_t size() const noexcept
        {
            return m_path.size();
        }
        META_NODISCARD META_INLINE bool empty() const noexcept
        {
            return m_path.empty();
        }

        META_NODISCARD META_INLINE String<> filename() const
        {
            size_t pos = m_path.rfind('/');
#ifdef _WIN32
            if (pos == String<>::npos)
                pos = m_path.rfind('\\');
#endif
            if (pos == String<>::npos)
                return m_path;
            return m_path.substr(pos + 1);
        }

        META_NODISCARD META_INLINE Path parentPath() const
        {
            size_t pos = m_path.rfind('/');
#ifdef _WIN32
            if (pos == String<>::npos)
                pos = m_path.rfind('\\');
#endif
            if (pos == String<>::npos)
                return Path("");
            return Path(m_path.substr(0, pos));
        }

        META_NODISCARD META_INLINE String<> extension() const
        {
            String<> file = filename();
            size_t pos = file.rfind('.');
            if (pos == String<>::npos)
                return "";
            return file.substr(pos);
        }

        META_INLINE bool operator==(const Path& rhs) const noexcept
        {
            return m_path == rhs.m_path;
        }
        META_INLINE bool operator!=(const Path& rhs) const noexcept
        {
            return !(*this == rhs);
        }

        META_INLINE std::string toString() const
        {
            return m_path.toString();
        }

        friend std::ostream& operator<<(std::ostream& os, const Path& path)
        {
            return os << path.c_str();
        }

    private:
        String<> m_path;

        // Build a new string with separators normalized for the current platform
        static META_INLINE String<> normalizeSeparators(const String<>& input)
        {
            String<> result;
            result.reserve(input.size());
            for (size_t i = 0; i < input.size(); ++i)
            {
#ifdef META_PLATFORM_WINDOWS
                char c = (input[i] == '/' || input[i] == '\\') ? '\\' : input[i];
#else
                char c = (input[i] == '/' || input[i] == '\\') ? '/' : input[i];
#endif
                result += c;
            }
            return result;
        }

        static META_INLINE String<> normalizeSeparators(String<>&& input)
        {
            String<> result;
            result.reserve(input.size());
            for (size_t i = 0; i < input.size(); ++i)
            {
#ifdef META_PLATFORM_WINDOWS
                char c = (input[i] == '/' || input[i] == '\\') ? '\\' : input[i];
#else
                char c = (input[i] == '/' || input[i] == '\\') ? '/' : input[i];
#endif
                result += c;
            }
            return result;
        }
    };
} // namespace meta
