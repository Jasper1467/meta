#pragma once

#include <chrono>
#include <fstream>
#include <iomanip>
#include <memory>
#include <meta/base/core/Console.hpp>
#include <meta/base/core/String.hpp>
#include <sstream>

namespace meta
{
    enum class LogLevel
    {
        Debug = 0,
        Info,
        Warning,
        Error,
        Off
    };

    class Logger
    {
    public:
        Logger() = default;

        void setLevel(LogLevel level)
        {
            m_level = level;
        }

        void includeTimestamps(bool enabled)
        {
            m_includeTimestamps = enabled;
        }

        void setFile(const meta::String<>& filepath)
        {
            m_file = std::make_unique<std::ofstream>(filepath.c_str(), std::ios::app);
        }

        template <typename... Args> void debug(Args&&... args)
        {
            if (m_level <= LogLevel::Debug)
                log("DEBUG", std::cout, std::forward<Args>(args)...);
        }

        template <typename... Args> void info(Args&&... args)
        {
            if (m_level <= LogLevel::Info)
                log("INFO", std::cout, std::forward<Args>(args)...);
        }

        template <typename... Args> void warning(Args&&... args)
        {
            if (m_level <= LogLevel::Warning)
                log("WARNING", std::cerr, std::forward<Args>(args)...);
        }

        template <typename... Args> void error(Args&&... args)
        {
            if (m_level <= LogLevel::Error)
                log("ERROR", std::cerr, std::forward<Args>(args)...);
        }

    private:
        LogLevel m_level = LogLevel::Info;
        bool m_includeTimestamps = false;
        std::unique_ptr<std::ofstream> m_file;

        template <typename... Args> void log(const meta::String<>& prefix, std::ostream& os, Args&&... args)
        {
            meta::String<> message = meta::format(std::forward<Args>(args)...);
            meta::String<> fullMessage;

            if (m_includeTimestamps)
            {
                fullMessage += "[" + currentTimestamp().toString() + "] ";
            }

            fullMessage += "[" + prefix.toString() + "] " + message.toString();

            os << fullMessage << "\n";

            if (m_file && m_file->is_open())
                (*m_file) << fullMessage << "\n";
        }

        meta::String<> currentTimestamp() const
        {
            auto now = std::chrono::system_clock::now();
            auto timeT = std::chrono::system_clock::to_time_t(now);
            auto localTime = *std::localtime(&timeT);

            std::ostringstream oss;
            oss << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");
            return meta::String<>(oss.str());
        }
    };
} // namespace meta
