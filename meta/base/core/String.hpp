#pragma once

#include <algorithm>
#include <array>
#include <cctype>
#include <cstddef>
#include <functional>
#include <iostream>
#include <memory>
#include <meta/base/core/Platform.hpp>
#include <string>
#include <string_view>

namespace meta
{
    template <size_t N = 128> class String
    {
    public:
        static constexpr size_t npos = SIZE_MAX;

        META_INLINE String() noexcept : m_size(0)
        {
        }

        META_INLINE String(const char* s)
        {
            size_t i = 0;
            while (s[i] != '\0' && i < N)
                m_buffer[i] = s[i], ++i;
            m_size = i;
            m_buffer[m_size] = '\0';
        }

        META_INLINE String(std::string_view sv)
        {
            if (sv.size() <= N)
            {
                m_size = sv.size();
                std::copy_n(sv.data(), m_size, m_buffer.data());
            }
            else
                m_runtime = std::make_unique<std::string>(sv);
        }

        META_INLINE String(const std::string& s) : String(std::string_view(s))
        {
        }
        META_INLINE String(std::string&& s) : String(std::string_view(s))
        {
        }

        META_INLINE String(const String& other) : m_size(other.m_size)
        {
            if (other.m_runtime)
                m_runtime = std::make_unique<std::string>(*other.m_runtime);
            else
                std::copy_n(other.m_buffer.data(), m_size, m_buffer.data());
        }

        META_INLINE String(String&& other) noexcept : m_size(other.m_size), m_runtime(std::move(other.m_runtime))
        {
            if (!m_runtime)
                std::copy_n(other.m_buffer.data(), m_size, m_buffer.data());
        }

        META_INLINE String& operator=(const String& other)
        {
            if (this == &other)
                return *this;
            m_size = other.m_size;
            if (other.m_runtime)
                m_runtime = std::make_unique<std::string>(*other.m_runtime);
            else
            {
                m_runtime = nullptr;
                std::copy_n(other.m_buffer.data(), m_size, m_buffer.data());
            }
            return *this;
        }

        META_INLINE String& operator=(String&& other) noexcept
        {
            if (this == &other)
                return *this;
            m_size = other.m_size;
            m_runtime = std::move(other.m_runtime);
            if (!m_runtime)
                std::copy_n(other.m_buffer.data(), m_size, m_buffer.data());
            return *this;
        }

        bool operator==(const String& rhs) const noexcept
        {
            if (m_size != rhs.m_size)
                return false;
            for (size_t i = 0; i < m_size; ++i)
                if ((*this)[i] != rhs[i])
                    return false;
            return true;
        }

        bool operator!=(const String& rhs) const noexcept
        {
            return !(*this == rhs);
        }

        template <size_t M> META_INLINE String<N>& operator+=(const String<M>& rhs)
        {
            if (m_runtime || m_size + rhs.size() > N)
            {
                if (!m_runtime)
                    m_runtime = std::make_unique<std::string>(std::string(m_buffer.data(), m_size));
                m_runtime->append(rhs.data(), rhs.size());
            }
            else
            {
                for (size_t i = 0; i < rhs.size(); ++i)
                    m_buffer[m_size + i] = rhs[i];
                m_size += rhs.size();
                m_buffer[m_size] = '\0';
            }
            return *this;
        }

        META_INLINE String<N>& operator+=(std::string_view sv)
        {
            if (m_runtime || m_size + sv.size() > N)
            {
                if (!m_runtime)
                    m_runtime = std::make_unique<std::string>(std::string(m_buffer.data(), m_size));
                m_runtime->append(sv.data(), sv.size());
            }
            else
            {
                for (size_t i = 0; i < sv.size(); ++i)
                    m_buffer[m_size + i] = sv[i];
                m_size += sv.size();
                m_buffer[m_size] = '\0';
            }
            return *this;
        }

        META_INLINE String<N>& operator+=(char c)
        {
            if (m_runtime)
                m_runtime->push_back(c);
            else if (m_size + 1 <= N)
                m_buffer[m_size++] = c, m_buffer[m_size] = '\0';
            else
                m_runtime = std::make_unique<std::string>(std::string(m_buffer.data(), m_size) + c);
            return *this;
        }

        template <size_t M> META_INLINE String<> operator+(const String<M>& rhs) const
        {
            return String<>(toString() + rhs.toString());
        }

        META_INLINE String<> operator+(std::string_view sv) const
        {
            return String<>(toString() + std::string(sv));
        }
        META_INLINE String<> operator+(const char* rhs) const
        {
            return String<>(toString() + std::string(rhs));
        }

        META_NODISCARD META_INLINE size_t size() const noexcept
        {
            return m_runtime ? m_runtime->size() : m_size;
        }
        META_NODISCARD META_INLINE bool empty() const noexcept
        {
            return size() == 0;
        }
        META_NODISCARD META_INLINE const char* data() const noexcept
        {
            return m_runtime ? m_runtime->data() : m_buffer.data();
        }
        META_NODISCARD META_INLINE const char* c_str() const noexcept
        {
            return data();
        }
        META_NODISCARD META_INLINE char operator[](size_t idx) const noexcept
        {
            return m_runtime ? (*m_runtime)[idx] : m_buffer[idx];
        }
        META_NODISCARD META_INLINE operator std::string_view() const noexcept
        {
            return m_runtime ? std::string_view(*m_runtime) : std::string_view(m_buffer.data(), m_size);
        }
        META_NODISCARD META_INLINE std::string toString() const
        {
            return m_runtime ? *m_runtime : std::string(m_buffer.data(), m_size);
        }

        META_NODISCARD META_INLINE String substr(size_t pos, size_t len = npos) const
        {
            if (pos >= size())
                return String{};
            size_t actualLen = std::min(len, size() - pos);
            return String(std::string_view(data() + pos, actualLen));
        }

        META_INLINE String& trim()
        {
            size_t start = 0;
            while (start < size() && std::isspace(static_cast<unsigned char>((*this)[start])))
                ++start;

            size_t end = size();
            while (end > start && std::isspace(static_cast<unsigned char>((*this)[end - 1])))
                --end;

            *this = substr(start, end - start);
            return *this;
        }

        META_NODISCARD META_INLINE size_t rfind(char c, size_t pos = npos) const noexcept
        {
            if (size() == 0)
                return npos;

            size_t start = (pos == npos || pos >= size()) ? size() - 1 : pos;
            for (size_t i = start + 1; i-- > 0;)
                if ((*this)[i] == c)
                    return i;
            return npos;
        }

        META_NODISCARD META_INLINE size_t rfind(const String<>& str, size_t pos = npos) const noexcept
        {
            if (str.empty() || size() < str.size())
                return npos;

            size_t start = (pos == npos || pos >= size() - str.size()) ? size() - str.size() : pos;
            for (size_t i = start + 1; i-- > 0;)
            {
                bool match = true;
                for (size_t j = 0; j < str.size(); ++j)
                    if ((*this)[i + j] != str[j])
                        match = false;
                if (match)
                    return i;
            }
            return npos;
        }

        friend std::ostream& operator<<(std::ostream& os, const String& str)
        {
            return os << str.c_str();
        }

        META_INLINE void reserve(size_t newCapacity)
        {
            if (newCapacity <= N)
                return;
            if (!m_runtime)
                m_runtime = std::make_unique<std::string>(std::string(m_buffer.data(), m_size));
            m_runtime->reserve(newCapacity);
        }

        META_NODISCARD
        META_INLINE char& front() noexcept
        {
            return m_runtime ? (*m_runtime)[0] : m_buffer[0];
        }

        META_NODISCARD
        META_INLINE const char& front() const noexcept
        {
            return m_runtime ? (*m_runtime)[0] : m_buffer[0];
        }

        META_NODISCARD
        META_INLINE char& back() noexcept
        {
            if (empty())
                throw std::out_of_range("String is empty");
            return m_runtime ? (*m_runtime)[m_runtime->size() - 1] : m_buffer[m_size - 1];
        }

        META_NODISCARD
        META_INLINE const char& back() const noexcept
        {
            if (empty())
                throw std::out_of_range("String is empty");
            return m_runtime ? (*m_runtime)[m_runtime->size() - 1] : m_buffer[m_size - 1];
        }

        void popBack()
        {
            if (m_size == 0)
                return; // nothing to pop

            if (m_runtime) // using heap string
            {
                m_runtime->pop_back();
                --m_size;
            }
            else // using small buffer
            {
                --m_size;
                m_buffer[m_size] = '\0'; // clear the last char
            }
        }

    private:
        size_t m_size = 0;
        std::array<char, N> m_buffer{};
        std::unique_ptr<std::string> m_runtime{};
    };
} // namespace meta

namespace std
{
    template <size_t N> struct hash<meta::String<N>>
    {
        size_t operator()(const meta::String<N>& s) const noexcept
        {
            // Use std::hash<std::string_view> on your string data
            return std::hash<std::string_view>{}(s);
        }
    };
} // namespace std
