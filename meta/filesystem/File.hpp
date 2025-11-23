#pragma once

#include <fstream>
#include <ios>
#include <meta/core/String.hpp>
#include <meta/filesystem/Path.hpp>

namespace meta
{
    class File
    {
    public:
        enum class Mode
        {
            Read,
            Write,
            ReadWrite,
            Append
        };

        // --- Constructors ---
        META_INLINE File() = default;

        explicit File(const Path& path, Mode mode)
        {
            open(path, mode);
        }

        // --- Destructor ---
        ~File()
        {
            close();
        }

        // --- Open file ---
        META_INLINE void open(const Path& path, Mode mode)
        {
            std::ios_base::openmode openMode = std::ios_base::binary;
            switch (mode)
            {
            case Mode::Read:
                openMode |= std::ios::in;
                break;
            case Mode::Write:
                openMode |= std::ios::out | std::ios::trunc;
                break;
            case Mode::ReadWrite:
                openMode |= std::ios::in | std::ios::out;
                break;
            case Mode::Append:
                openMode |= std::ios::out | std::ios::app;
                break;
            }

            m_stream.open(path.c_str(), openMode);

            if (!m_stream.is_open())
                throw std::runtime_error("Failed to open file: " + path.toString());
        }

        // --- Close file ---
        META_INLINE void close()
        {
            if (m_stream.is_open())
                m_stream.close();
        }

        // --- Check status ---
        META_NODISCARD META_INLINE bool isOpen() const
        {
            return m_stream.is_open();
        }

        META_NODISCARD META_INLINE bool eof() const
        {
            return m_stream.eof();
        }

        // --- Read ---
        META_INLINE String<> readAll()
        {
            if (!m_stream.is_open())
                throw std::runtime_error("File not open for reading");

            m_stream.seekg(0, std::ios::end);
            size_t size = static_cast<size_t>(m_stream.tellg());
            m_stream.seekg(0, std::ios::beg);

            String<> result;
            result.reserve(size); // Optional, if String has reserve
            char c;
            while (m_stream.get(c))
                result += c;

            return result;
        }

        // --- Write ---
        META_INLINE void write(const String<>& data)
        {
            if (!m_stream.is_open())
                throw std::runtime_error("File not open for writing");

            m_stream.write(data.data(), data.size());
        }

    private:
        std::fstream m_stream;
    };
} // namespace meta
