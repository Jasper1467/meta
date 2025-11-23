#include <meta/base/app/Logger.hpp>
#include <meta/base/app/SettingsManager.hpp>
#include <meta/base/core/Console.hpp>
#include <meta/base/core/FeatureCheck.hpp>
#include <meta/base/core/Format.hpp>
#include <meta/base/core/Platform.hpp>
#include <meta/base/filesystem/Path.hpp>
#include <meta/base/math/Constants.hpp>

#include <meta/gui/Button.hpp>
#include <meta/gui/HorizontalLayout.hpp>
#include <meta/gui/Label.hpp>
#include <meta/gui/Slider.hpp>
#include <meta/gui/Theme.hpp>
#include <meta/gui/VerticalLayout.hpp>
#include <meta/gui/Window.hpp>

int main()
{
    int x = 42;
    double pi = meta::PI;
    meta::String<> name("Alice");

    meta::println(meta::format("Hello, ", name, "! x = ", x, ", pi = ", pi));

    meta::Path settingsPath("settings.ini");
    meta::SettingsManager settings(settingsPath);

    settings.set("User", "Name", name);
    settings.set("User", "Age", 30);
    settings.set("Math", "PI", pi);

    if (!settings.save())
    {
        meta::errorln("Failed to save settings!");
        return 1;
    }

    meta::SettingsManager loadedSettings(settingsPath);
    if (!loadedSettings.load())
    {
        meta::errorln("Failed to load settings!");
        return 1;
    }

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

    meta::gui::Window window("Nested Layouts with Themed Buttons", 600, 400);

    auto theme = std::make_shared<meta::gui::Theme>();

    window.setTheme(theme);
    window.setBackgroundColor(theme->backgroundColor.r, theme->backgroundColor.g, theme->backgroundColor.b,
                              theme->backgroundColor.a);

    meta::gui::Button btn1("Button 1");
    meta::gui::Button btn2("Button 2");
    meta::gui::Button btn3("Button 3");
    meta::gui::Button btn4("Button 4");

    meta::gui::Label label1("Label 1");

    meta::gui::Slider slider1("Slider 1");

    btn1.setOnClick([&]() { meta::println("Button 1 clicked!"); });
    btn2.setOnClick([&]() { meta::println("Button 2 clicked!"); });
    btn3.setOnClick([&]() { meta::println("Button 3 clicked!"); });
    btn4.setOnClick([&]() { meta::println("Button 4 clicked!"); });

    auto mainLayout = std::make_shared<meta::gui::VerticalLayout>(10, 10);   // spacing=10, padding=10
    auto nestedLayout = std::make_shared<meta::gui::HorizontalLayout>(5, 5); // spacing=5, padding=5

    nestedLayout->addWidget(&btn3);
    nestedLayout->addWidget(&btn4);
    nestedLayout->addWidget(&label1);
    nestedLayout->addWidget(&slider1);

    mainLayout->addWidget(&btn1);
    mainLayout->addWidget(&btn2);
    mainLayout->addLayout(nestedLayout);

    window.setLayout(mainLayout);

    window.run(
        [&](bool& running)
        {
            // No extra logic needed here; Window handles event polling and rendering
        });

    return 0;
}
