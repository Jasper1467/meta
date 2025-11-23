#pragma once
#include <memory>
#include <meta/base/filesystem/Path.hpp>

namespace meta::graphics
{

    class Texture
    {
    public:
        virtual ~Texture() = default;
        virtual int Width() const = 0;
        virtual int Height() const = 0;

        // Load texture from a file path (accepts meta::Path directly)
        static std::unique_ptr<Texture> LoadFromFile(const meta::Path& path);
    };

} // namespace meta::graphics
