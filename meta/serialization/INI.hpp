#pragma once

#include <meta/core/Format.hpp>
#include <meta/core/String.hpp>
#include <meta/filesystem/File.hpp>
#include <sstream>
#include <unordered_map>

namespace meta
{
    class INI
    {
    public:
        using Section = std::unordered_map<meta::String<>, meta::String<>>;
        using Data = std::unordered_map<meta::String<>, Section>;

        INI() = default;

        bool load(const meta::Path& filepath)
        {
            try
            {
                meta::File file(filepath, meta::File::Mode::Read);
                auto content = file.readAll();
                parse(content);
                return true;
            }
            catch (...)
            {
                return false;
            }
        }

        bool save(const meta::Path& filepath) const
        {
            try
            {
                meta::File file(filepath, meta::File::Mode::Write);
                meta::String<> content;

                for (const auto& [section, kv] : m_data)
                {
                    if (!section.empty())
                        content += meta::format("[", section, "]\n");

                    for (const auto& [key, value] : kv)
                        content += meta::format(key, "=", value, "\n");

                    content += "\n";
                }

                file.write(content);
                return true;
            }
            catch (...)
            {
                return false;
            }
        }

        template <typename T> void set(const meta::String<>& section, const meta::String<>& key, const T& value)
        {
            m_data[section][key] = meta::format(value);
        }

        template <typename T>
        T get(const meta::String<>& section, const meta::String<>& key, const T& defaultValue = {}) const
        {
            auto secIt = m_data.find(section);
            if (secIt != m_data.end())
            {
                auto keyIt = secIt->second.find(key);
                if (keyIt != secIt->second.end())
                {
                    if constexpr (std::is_same_v<T, meta::String<>>)
                    {
                        return keyIt->second;
                    }
                    else
                    {
                        std::istringstream iss(keyIt->second.toString());
                        T result{};
                        if (iss >> result)
                            return result;
                    }
                }
            }
            return defaultValue;
        }

        bool has(const meta::String<>& section, const meta::String<>& key) const
        {
            auto secIt = m_data.find(section);
            return secIt != m_data.end() && secIt->second.find(key) != secIt->second.end();
        }

        void clear()
        {
            m_data.clear();
        }

    private:
        void parse(const meta::String<>& content)
        {
            m_data.clear();
            meta::String<> currentSection;

            std::istringstream iss(content.toString());
            std::string rawLine;

            while (std::getline(iss, rawLine))
            {
                meta::String<> line(rawLine); // convert std::string to meta::String
                line.trim();

                if (line.empty() || line[0] == ';' || line[0] == '#')
                    continue;

                if (line.front() == '[' && line.back() == ']')
                {
                    currentSection = line.substr(1, line.size() - 2).trim();
                }
                else
                {
                    auto eqPos = line.rfind('=');
                    if (eqPos != meta::String<>::npos)
                    {
                        meta::String<> key = line.substr(0, eqPos).trim();
                        meta::String<> value = line.substr(eqPos + 1).trim();
                        m_data[currentSection][key] = value;
                    }
                }
            }
        }

        Data m_data;
    };
} // namespace meta::serialization
