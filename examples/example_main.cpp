#include <meta/app/SettingsManager.hpp>
#include <meta/core/Console.hpp>
#include <meta/core/FeatureCheck.hpp>
#include <meta/core/Format.hpp>
#include <meta/core/Platform.hpp>
#include <meta/filesystem/Path.hpp>
#include <meta/math/Constants.hpp>

int main()
{
    int x = 42;
    double pi = meta::PI;
    meta::String<> name("Alice");

    auto msg = meta::format("Hello, ", name, "! x = ", x, ", pi = ", pi, "\n");
    meta::println(msg);

    meta::Path settingsPath("settings.ini");
    meta::SettingsManager settings(settingsPath);

    // Set some values
    settings.set("User", "Name", name);
    settings.set("User", "Age", 30);
    settings.set("Math", "PI", pi);

    // Save settings
    if (!settings.save())
    {
        meta::errorln("Failed to save settings!");
        return 1;
    }

    // Load settings
    meta::SettingsManager loadedSettings(settingsPath);
    if (!loadedSettings.load())
    {
        meta::errorln("Failed to load settings!");
        return 1;
    }

    // Access loaded values
    meta::String<> loadedName = loadedSettings.get<meta::String<>>("User", "Name", "Unknown");
    int loadedAge = loadedSettings.get<int>("User", "Age", 0);
    double loadedPi = loadedSettings.get<double>("Math", "PI", 0.0);

    meta::println("Loaded from SettingsManager: Name=", loadedName, ", Age=", loadedAge, ", PI=", loadedPi);

    return 0;
}
