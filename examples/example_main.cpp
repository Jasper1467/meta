#include <meta/base/app/Logger.hpp>
#include <meta/base/app/SettingsManager.hpp>
#include <meta/base/core/Console.hpp>
#include <meta/base/core/FeatureCheck.hpp>
#include <meta/base/core/Format.hpp>
#include <meta/base/core/Platform.hpp>
#include <meta/base/filesystem/Path.hpp>
#include <meta/base/math/Constants.hpp>



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

    meta::Logger logger;
    logger.setLevel(meta::LogLevel::Debug);
    logger.includeTimestamps(true);
    logger.setFile("app.log");

    logger.debug("Debug message with timestamp");
    logger.info("Application started at x=", 42);
    logger.warning("This might be risky");
    logger.error("Something went wrong!");

    return 0;
}
