#pragma once

#include <meta/core/Console.hpp>
#include <meta/filesystem/Path.hpp>
#include <meta/serialization/INI.hpp>

namespace meta
{
    class SettingsManager
    {
    public:
        SettingsManager() = default;

        explicit SettingsManager(const meta::Path& path) : m_filePath(path)
        {
        }

        bool load()
        {
            if (!m_filePath.empty() && m_ini.load(m_filePath))
            {
                meta::println("Settings loaded from ", m_filePath);
                return true;
            }

            meta::errorln("Failed to load settings from ", m_filePath);
            return false;
        }

        bool save() const
        {
            if (!m_filePath.empty() && m_ini.save(m_filePath))
            {
                meta::println("Settings saved to ", m_filePath);
                return true;
            }

            meta::errorln("Failed to save settings to ", m_filePath);
            return false;
        }

        template <typename T> void set(const meta::String<>& section, const meta::String<>& key, const T& value)
        {
            m_ini.set(section, key, value);
        }

        template <typename T>
        T get(const meta::String<>& section, const meta::String<>& key, const T& defaultValue = {}) const
        {
            return m_ini.get<T>(section, key, defaultValue);
        }

        meta::INI& ini()
        {
            return m_ini;
        }

        const meta::INI& ini() const
        {
            return m_ini;
        }

        void setFilePath(const meta::Path& path)
        {
            m_filePath = path;
        }

    private:
        meta::Path m_filePath;
        meta::INI m_ini;
    };
} // namespace meta
