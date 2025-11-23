#pragma once

#include <meta/base/core/String.hpp>
#include <sstream>
#include <type_traits>

namespace meta
{
    // --- Convert any value to meta::String<> ---
    template <typename T> inline meta::String<> to_meta_string(const T& value)
    {
        if constexpr (std::is_same_v<T, meta::String<>>)
        {
            return value;
        }
        else if constexpr (std::is_convertible_v<T, std::string>)
        {
            return meta::String<>(value);
        }
        else
        {
            std::ostringstream oss;
            oss << value;
            return meta::String<>(oss.str());
        }
    }

    // --- Variadic format function ---
    template <typename... Args> inline meta::String<> format(Args&&... args)
    {
        meta::String<> result;
        ((result += to_meta_string(std::forward<Args>(args))), ...); // fold expression
        return result;
    }

} // namespace meta
