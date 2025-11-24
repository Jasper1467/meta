#include <meta/base/app/Logger.hpp>
#include <meta/gui/TabContainer.hpp>
#include <meta/gui/Theme.hpp>
#include <meta/gui/Window.hpp>
#include <meta/gui/layouts/VerticalLayout.hpp>
#include <meta/gui/widgets/Button.hpp>
#include <meta/gui/widgets/CheckBox.hpp>
#include <meta/gui/widgets/Slider.hpp>
#include <meta/gui/widgets/Tab.hpp>
#include <meta/gui/widgets/TextBox.hpp>
#include <meta/gui/widgets/Toggle.hpp>

int main()
{
    // Initialize logger
    meta::Logger logger;
    logger.setLevel(meta::LogLevel::Debug);
    logger.includeTimestamps(true);
    logger.setFile("widget_demo.log");

    meta::println("Starting Widget Demo...");

    // Create main window
    meta::gui::Window window("Widget Demo", 800, 600);

    // Create TabContainer
    auto tabContainer = std::make_shared<meta::gui::TabContainer>(10, 10, 780, 580);

    // ---------- Page 1: Controls ----------
    auto page1 = std::make_shared<meta::gui::VerticalLayout>(10, 10);

    // Button
    auto button1 = std::make_shared<meta::gui::Button>("Click Me");
    button1->clicked.connect([]() { meta::println("Button clicked!"); });

    // Slider
    auto slider1 = std::make_shared<meta::gui::Slider>("Volume");
    slider1->setValue(50);
    slider1->valueChanged.connect([](int value) { meta::println("Slider value:", value); });

    page1->addWidget(button1.get());
    page1->addWidget(slider1.get());

    auto tab1 = std::make_shared<meta::gui::Tab>("Controls");

    // ---------- Page 2: Inputs ----------
    auto page2 = std::make_shared<meta::gui::VerticalLayout>(10, 10);

    // CheckBox
    auto checkBox = std::make_shared<meta::gui::CheckBox>("Check me");
    checkBox->toggled.connect([](bool checked) { meta::println("CheckBox toggled:", checked); });

    // Toggle
    auto toggle = std::make_shared<meta::gui::Toggle>("Toggle me");
    toggle->toggled.connect([](bool state) { meta::println("Toggle state:", state); });

    page2->addWidget(checkBox.get());
    page2->addWidget(toggle.get());

    auto tab2 = std::make_shared<meta::gui::Tab>("Inputs");

    // Add tabs to TabContainer
    tabContainer->addTab(tab1, page1);
    tabContainer->addTab(tab2, page2);

    // Tab change signal
    tabContainer->tabChanged.connect([](int index) { meta::println("Active tab changed to index:", index); });

    // Set layout
    window.setLayout(tabContainer);

    // Run main loop
    window.run(
        [](bool& running)
        {
            // Optional per-frame logic
        });

    return 0;
}
