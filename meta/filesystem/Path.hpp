#pragma once

#include <cstddef>
#include <iostream>
#include <meta/core/String.hpp>

namespace meta
{
    class Path
    {
    public:
        META_INLINE Path() = default;

        META_INLINE Path(const String<>& str) : m_path(str)
        {
        }
        META_INLINE Path(String<>&& str) : m_path(std::move(str))
        {
        }
        META_INLINE Path(const char* s) : m_path(s)
        {
        }
        META_INLINE Path(std::string_view sv) : m_path(sv)
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

        META_INLINE Path operator/(const Path& rhs) const
        {
            if (m_path.empty())
                return rhs;
            if (rhs.m_path.empty())
                return *this;

            String<> combined = m_path;
            if (combined[combined.size() - 1] != '/')
                combined += "/";
            combined += rhs.m_path;
            return Path(std::move(combined));
        }

        META_INLINE Path& operator/=(const Path& rhs)
        {
            *this = *this / rhs;
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
            if (pos == String<>::npos)
                return m_path;
            return m_path.substr(pos + 1);
        }

        META_NODISCARD META_INLINE String<> extension() const
        {
            String<> file = filename();
            size_t pos = file.rfind('.');
            if (pos == String<>::npos)
                return "";
            return file.substr(pos);
        }

        META_NODISCARD META_INLINE Path parent_path() const
        {
            size_t pos = m_path.rfind('/');
            if (pos == String<>::npos)
                return Path("");
            return Path(m_path.substr(0, pos));
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
            return m_path.toString(); // Convert meta::String to std::string
        }
 

        friend std::ostream& operator<<(std::ostream& os, const Path& path)
        {
            return os << path.c_str();
        }

    private:
        String<> m_path;
    };
} // namespace meta
