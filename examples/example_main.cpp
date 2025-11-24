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
    meta::Logger logger;
    logger.setLevel(meta::LogLevel::Debug);
    logger.includeTimestamps(true);
    logger.setFile("tab_demo.log");

    meta::println("Starting Tab System Demo...");

    meta::gui::Window window("Tab System Demo", 800, 600);

    // Create TabContainer
    auto tabContainer = std::make_shared<meta::gui::TabContainer>(10, 10, 780, 580);

    // ---------- Page 1 ----------
    auto page1 = std::make_shared<meta::gui::VerticalLayout>(10, 10);

    auto btn1 = std::make_shared<meta::gui::Button>("Button 1");
    btn1->clicked.connect([]() { meta::println("Button 1 clicked on Page 1!"); });

    auto slider1 = std::make_shared<meta::gui::Slider>("Volume");
    slider1->setValue(50);
    slider1->valueChanged.connect([](int v) { meta::println("Slider value:", v); });

    page1->addWidget(btn1.get());
    page1->addWidget(slider1.get());

    auto tab1 = std::make_shared<meta::gui::Tab>("Controls");

    // ---------- Page 2 ----------
    auto page2 = std::make_shared<meta::gui::VerticalLayout>(10, 10);

    auto textBox1 = std::make_shared<meta::gui::TextBox>("Enter text:");
    textBox1->textChanged.connect([](const meta::String<>& t) { meta::println("Text changed:", t); });

    auto checkBox1 = std::make_shared<meta::gui::CheckBox>("Check me");
    checkBox1->toggled.connect([](bool checked) { meta::println("CheckBox toggled:", checked); });

    auto toggle1 = std::make_shared<meta::gui::Toggle>("Toggle me");
    toggle1->toggled.connect([](bool state) { meta::println("Toggle state:", state); });

    page2->addWidget(textBox1.get());
    page2->addWidget(checkBox1.get());
    page2->addWidget(toggle1.get());

    auto tab2 = std::make_shared<meta::gui::Tab>("Settings");

    // Add tabs to container
    tabContainer->addTab(tab1, page1);
    tabContainer->addTab(tab2, page2);

    // Connect tab change signal
    tabContainer->tabChanged.connect([](int index) { meta::println("Active Tab Changed to index:", index); });

    // Set TabContainer as main layout
    window.setLayout(tabContainer);

    // Main loop
    window.run(
        [](bool& running)
        {
            // Per-frame logic here if needed
        });

    return 0;
}
