#include "Texture.h"
#include <stdexcept>

Texture::Texture(const std::string& fileName)
{
    if (!loadFromFile(fileName)) throw std::runtime_error("Cannot load texture: " + fileName);
    generateMipmap();
    setSmooth(true);
}
Texture::Texture() = default;
