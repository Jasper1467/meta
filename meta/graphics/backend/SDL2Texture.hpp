#pragma once

#include "../Texture.hpp"
#include <SDL.h>
#include <meta/base/filesystem/Path.hpp>

namespace meta::graphics
{

    class SDL2Texture final : public Texture
    {
    public:
        SDL2Texture(SDL_Renderer* renderer, const meta::Path& path);
        ~SDL2Texture() override;

        int Width() const override;
        int Height() const override;

        SDL_Texture* GetSDLTexture() const
        {
            return texture_;
        }

    private:
        SDL_Texture* texture_ = nullptr;
        int width_ = 0;
        int height_ = 0;
    };

} // namespace meta::graphics
