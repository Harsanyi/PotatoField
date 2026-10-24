#include "Sprite.h"
#include "Texture.h"
#include "Log.h"
#include "Utils.h"

Sprite::Sprite(const std::string& fileName) : texture(fileName)
{
    setTexture(texture);
    setTextureRect(sf::IntRect(0, 0, texture.getSize().x, texture.getSize().y));
    setOrigin(static_cast<sf::Vector2f>(texture.getSize()) * 0.5f);
}

Sprite::Sprite(const Texture& texture)
{
    setTexture(texture);
    setTextureRect(sf::IntRect(0, 0, texture.getSize().x, texture.getSize().y));
    setOrigin(static_cast<sf::Vector2f>(texture.getSize()) * 0.5f);
}

sf::Vector2f Sprite::getSize()
{
    auto size = texture.getSize();
    auto scale = this->getScale();
    return {size.x * scale.x, size.y * scale.y};
}

void Sprite::setSize(const sf::Vector2f& size)
{
    auto tSize = texture.getSize();
    sf::Vector2f scale = {size.x / tSize.x, size.y / tSize.y};
    this->setScale(scale);
}

void Sprite::setOriginNormal(const sf::Vector2f& normOrig)
{
    this->setOrigin({normOrig.x * getSize().x, normOrig.y * getSize().y});
}
