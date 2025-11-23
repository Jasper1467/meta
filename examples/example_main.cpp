#include <iostream>
#include <meta/core/Platform.hpp>
#include <meta/core/FeatureCheck.hpp>
#include <meta/core/Format.hpp>
#include <meta/math/Constants.hpp>

int main()
{
    int x = 42;
    double pi = meta::PI;
    meta::String<> name("Alice");

    auto msg = meta::format("Hello, ", name, "! x = ", x, ", pi = ", pi, "\n");
    std::cout << msg << std::endl;

    return 0;
}
