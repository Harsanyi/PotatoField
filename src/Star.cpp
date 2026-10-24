#include "Star.h"
#include "Assets.h"
#include "Game.h"
#include "Player.h"

std::vector<Star> Star::stars;
sf::VertexArray Star::vertices{sf::Quads};
std::unique_ptr<Texture> Star::texture;
sf::RenderStates Star::state;

void Star::init()
{
    texture = std::make_unique<Texture>(Assets::file("img/star.png"));
    state = sf::RenderStates{};
    state.texture = texture.get();
    state.transform.translate(Config::SCREEN_WIDTH * 0.5f, Config::SCREEN_HEIGHT * 0.5f);
}
void Star::create(int count)
{
    stars.clear();
    stars.resize(static_cast<std::size_t>(count));
    vertices.resize(stars.size() * 4);
    const auto size = texture->getSize();
    for (std::size_t i = 0; i < stars.size(); ++i)
    {
        vertices[i * 4].texCoords = {0, 0};
        vertices[i * 4 + 1].texCoords = {float(size.x), 0};
        vertices[i * 4 + 2].texCoords = {float(size.x), float(size.y)};
        vertices[i * 4 + 3].texCoords = {0, float(size.y)};
    }
}
void Star::draw()
{
    constexpr glm::vec2 corners[]{{-0.5f, -0.5f}, {0.5f, -0.5f}, {0.5f, 0.5f}, {-0.5f, 0.5f}};
    const auto inverseCamera = glm::inverse(Player::main->rotation);
    for (std::size_t i = 0; i < stars.size(); ++i)
    {
        const auto local = inverseCamera * stars[i].rotation * glm::vec3(0, 0, 10000);
        if (local.z <= 1.0f)
        {
            for (std::size_t corner = 0; corner < 4; ++corner) vertices[i * 4 + corner].color = sf::Color::Transparent;
            continue;
        }
        const auto projected = Player::main->localToScreenPosition(local);
        for (std::size_t corner = 0; corner < 4; ++corner)
        {
            vertices[i * 4 + corner].position = {projected.x + corners[corner].x * stars[i].size,
                                                projected.y + corners[corner].y * stars[i].size};
            vertices[i * 4 + corner].color = stars[i].color;
        }
    }
    // A second shared vertex array renders the distant, rotation-only starfield.
    Game::window->draw(vertices, state);
}
void Star::shutdown()
{
    stars.clear();
    vertices.clear();
    state.texture = nullptr;
    texture.reset();
}
