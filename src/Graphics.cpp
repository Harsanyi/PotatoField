#include "Graphics.h"
#include "Assets.h"
#include "Config.h"
#include "Game.h"
#include "Player.h"
#include "Utils.h"
#include "WorldMath.h"
#include <algorithm>
#include <numeric>

std::unique_ptr<Texture> Graphics::spriteSheet;
sf::RenderStates Graphics::states;
sf::VertexArray Graphics::vertices{sf::Quads};
std::vector<std::size_t> Graphics::indexes;
std::vector<glm::vec3> Graphics::localPositions;
Event<Graphics*> Graphics::onSpawn;
Event<Graphics*> Graphics::onDestroy;
std::vector<Graphics*> Graphics::instances;
std::vector<Graphics*> Graphics::markedForDestruction;
bool Graphics::shuttingDown = false;

Graphics::Graphics()
{
    instances.push_back(this);
    onSpawn.Invoke(this);
}

Graphics::~Graphics()
{
    onDestroy.Invoke(this);
    remove(instances, this);
    remove(markedForDestruction, this);
}

void Graphics::Destroy()
{
    if (!pendingDestruction)
    {
        pendingDestruction = true;
        markedForDestruction.push_back(this);
    }
}

void Graphics::init()
{
    shuttingDown = false;
    spriteSheet = std::make_unique<Texture>(Assets::file("img/spriteSheet.png"));
    states = sf::RenderStates{};
    states.texture = spriteSheet.get();
    states.transform.translate(Config::SCREEN_WIDTH / 2.0f, Config::SCREEN_HEIGHT / 2.0f);
}

void Graphics::deleteMarkedForDestruction()
{
    // Destruction callbacks can spawn new entities or queue other objects. Process only this batch.
    auto pending = std::move(markedForDestruction);
    markedForDestruction.clear();
    for (auto* object : pending)
        if (std::find(instances.begin(), instances.end(), object) != instances.end()) delete object;
}

void Graphics::shutdown()
{
    shuttingDown = true;
    onSpawn.Clear();
    onDestroy.Clear();
    markedForDestruction.clear();
    while (!instances.empty()) delete instances.back();
    vertices.clear();
    localPositions.clear();
    indexes.clear();
    states.texture = nullptr;
    spriteSheet.reset();
}

void Graphics::updateVertices()
{
    constexpr glm::vec2 corners[]{{-1, -1}, {1, -1}, {1, 1}, {-1, 1}};
    for (std::size_t p = 0; p < indexes.size(); ++p)
    {
        const auto index = indexes[p];
        const auto& local = localPositions[index];
        const auto* object = instances[index];
        const auto offset = p * 4;
        if (object == Player::main || object->pendingDestruction || local.z <= 1.0f)
        {
            for (std::size_t corner = 0; corner < 4; ++corner)
            {
                vertices[offset + corner].position = {};
                vertices[offset + corner].color = sf::Color::Transparent;
            }
            continue;
        }
        const auto screen = Player::main->localToScreenPosition(local);
        const float depthScale = std::clamp(Config::DEPTH_SCALE / glm::length(local), 0.0f, 1.0f);
        const float halfSize = object->size * depthScale * 0.5f;
        const float radians = glm::radians(object->rotation);
        const auto shade = static_cast<sf::Uint8>(255 * (1 - glm::pow(1 - depthScale, Config::LIGHT_FALL_RATE)));
        for (std::size_t corner = 0; corner < 4; ++corner)
        {
            vertices[offset + corner].position = v2f(glm::vec2(screen) + rotate(corners[corner] * halfSize, radians));
            vertices[offset + corner].color = {shade, shade, shade};
        }
    }
}

void Graphics::fillTextureCoords()
{
    const auto dimensions = spriteSheet->getSize();
    for (std::size_t p = 0; p < indexes.size(); ++p)
    {
        const auto uv = instances[indexes[p]]->getUVs();
        const float left = uv.x * dimensions.x, top = uv.y * dimensions.y;
        const float right = uv.z * dimensions.x, bottom = uv.w * dimensions.y;
        vertices[p * 4].texCoords = {left, top};
        vertices[p * 4 + 1].texCoords = {right, top};
        vertices[p * 4 + 2].texCoords = {right, bottom};
        vertices[p * 4 + 3].texCoords = {left, bottom};
    }
}

void Graphics::updateLocalPositions()
{
    for (std::size_t i = 0; i < instances.size(); ++i)
        localPositions[i] = Player::main->worldToLocalPosition(instances[i]->position);
}

bool Graphics::overlaps(const Graphics& first, const Graphics& second)
{
    const float radius = (first.size + second.size) * 0.5f;
    const auto delta = wrappedDelta(first.position - second.position);
    return glm::dot(delta, delta) < radius * radius;
}

void Graphics::updateCollisions()
{
    // World-space collision pairs are independent of camera shake and render sorting.
    const auto snapshot = instances;
    for (std::size_t i = 0; i < snapshot.size(); ++i)
        for (std::size_t j = i + 1; j < snapshot.size(); ++j)
        {
            auto* first = snapshot[i];
            auto* second = snapshot[j];
            if (first->pendingDestruction || second->pendingDestruction) continue;
            if (!first->isActiveCollider() && !second->isActiveCollider()) continue;
            if (overlaps(*first, *second))
            {
                first->onOverlap(second);
                second->onOverlap(first);
            }
        }
}

void Graphics::sortByLocalZ()
{
    std::sort(indexes.begin(), indexes.end(),
        [](std::size_t a, std::size_t b) { return localPositions[a].z > localPositions[b].z; });
}

void Graphics::updateGraphicsBehaviours()
{
    const auto snapshot = instances;
    for (auto* object : snapshot) if (!object->pendingDestruction) object->update();
}

void Graphics::fixVectorSizes()
{
    indexes.resize(instances.size());
    std::iota(indexes.begin(), indexes.end(), std::size_t{0});
    localPositions.resize(instances.size());
    vertices.resize(instances.size() * 4);
}

void Graphics::draw() { Game::window->draw(vertices, states); }

void Graphics::updateBehaviours()
{
    deleteMarkedForDestruction();
    updateGraphicsBehaviours();
    fixVectorSizes();
    updateLocalPositions();
    updateCollisions();
    sortByLocalZ();
    // One shared quad array retains the original batched sprite rendering approach.
    updateVertices();
    fillTextureCoords();
}
