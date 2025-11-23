#include <meta/base/app/Logger.hpp>
#include <meta/base/app/SettingsManager.hpp>
#include <meta/base/core/Console.hpp>
#include <meta/base/core/Format.hpp>
#include <meta/base/filesystem/Path.hpp>
#include <meta/base/math/Constants.hpp>

#include <meta/gui/Button.hpp>
#include <meta/gui/HorizontalLayout.hpp>
#include <meta/gui/Slider.hpp>
#include <meta/gui/TextBox.hpp>
#include <meta/gui/Theme.hpp>
#include <meta/gui/VerticalLayout.hpp>
#include <meta/gui/Window.hpp>
#include <meta/gui/CheckBox.hpp>
#include <meta/gui/Toggle.hpp>

int main()
{
    meta::Logger logger;
    logger.setLevel(meta::LogLevel::Debug);
    logger.includeTimestamps(true);
    logger.setFile("app.log");

    meta::println("Starting GUI demo...");

    meta::gui::Window window("Nested Layouts with Themed Widgets", 600, 400);

    auto theme = std::make_shared<meta::gui::Theme>(); 

    window.setTheme(theme);

    meta::gui::Button btn1("Button 1");
    meta::gui::Button btn2("Button 2");
    meta::gui::Button btn3("Button 3");
    meta::gui::Button btn4("Button 4");

    meta::gui::Slider slider1("Volume");
    meta::gui::TextBox textBox1("Text box");
    meta::gui::CheckBox checkBox1("Check box");
    meta::gui::Toggle toggle1("Toggle");

    btn1.setOnClick([&]() { meta::println("Button 1 clicked!"); });
    btn2.setOnClick([&]() { meta::println("Button 2 clicked!"); });
    btn3.setOnClick([&]() { meta::println("Button 3 clicked!"); });
    btn4.setOnClick([&]() { meta::println("Button 4 clicked!"); });

    auto mainLayout = std::make_shared<meta::gui::VerticalLayout>(10, 10);   // spacing, padding
    auto nestedLayout = std::make_shared<meta::gui::HorizontalLayout>(5, 5); // spacing, padding

    nestedLayout->addWidget(&btn3);
    nestedLayout->addWidget(&btn4);
    nestedLayout->addWidget(&slider1);
    nestedLayout->addWidget(&checkBox1);
    nestedLayout->addWidget(&toggle1);

    mainLayout->addWidget(&btn1);
    mainLayout->addWidget(&btn2);
    mainLayout->addWidget(&textBox1);
    mainLayout->addLayout(nestedLayout);

    window.setLayout(mainLayout);

    window.run([&](bool& running) {});

    return 0;
}
