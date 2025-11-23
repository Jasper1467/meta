#include <SDL_image.h>
#include "SDL2Texture.hpp"

namespace meta::graphics
{
    SDL2Texture::SDL2Texture(SDL_Renderer* renderer, const meta::Path& path)
    {
        SDL_Surface* surface = IMG_Load(path.c_str()); // Path -> meta::String -> c_str()
        if (!surface)
            throw std::runtime_error("Failed to load texture: " + path.toString());

        texture_ = SDL_CreateTextureFromSurface(renderer, surface);
        width_ = surface->w;
        height_ = surface->h;
        SDL_FreeSurface(surface);
    }

    SDL2Texture::~SDL2Texture()
    {
        if (texture_)
            SDL_DestroyTexture(texture_);
    }

    int SDL2Texture::Width() const
    {
        return width_;
    }
    int SDL2Texture::Height() const
    {
        return height_;
    }

    std::unique_ptr<Texture> Texture::LoadFromFile(const meta::Path& path)
    {
        // You can manage a global renderer, or pass it via parameter
        extern SDL_Renderer* gRenderer;
        return std::make_unique<SDL2Texture>(gRenderer, path);
    }
} // namespace meta::graphics
