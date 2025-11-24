#include <meta/base/app/Logger.hpp>
#include <meta/base/core/Console.hpp>

int main()
{
    meta::Logger logger;
    logger.setLevel(meta::LogLevel::Debug);
    logger.includeTimestamps(true);
    logger.setFile("console_demo.log");

    meta::println("Starting Console Color Demo...");

    // Standard output
    meta::println("This is normal text.");
    meta::print("This is inline text... ");
    meta::println("and this is on the same line.");

    // Colored output
    meta::printlnColor(meta::ConsoleColor::Red, "Red text");
    meta::printlnColor(meta::ConsoleColor::Green, "Green text");
    meta::printlnColor(meta::ConsoleColor::Blue, "Blue text");
    meta::printlnColor(meta::ConsoleColor::Yellow, "Yellow text");
    meta::printlnColor(meta::ConsoleColor::Magenta, "Magenta text");
    meta::printlnColor(meta::ConsoleColor::Cyan, "Cyan text");
    meta::printlnColor(meta::ConsoleColor::White, "White text");

    // Error output
    meta::errorlnColor(meta::ConsoleColor::Red, "This is an error message in red!");
    meta::errorln("This is a standard error message.");

    // Debug output (only prints in debug builds)
    meta::debuglnColor(meta::ConsoleColor::Cyan, "Debug message in cyan");
    meta::debugln("Standard debug message");

    // Mixing colors and standard output
    meta::printColor(meta::ConsoleColor::Green, "Inline green text");
    meta::println(" back to normal.");

    // Reset to default color
    meta::printlnColor(meta::ConsoleColor::Default, "Back to default color.");

    return 0;
}
