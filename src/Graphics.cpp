#include "graphics.h"
#include "config.h"
#include "Event.h"
#include <iostream>
#include "Log.h"
#include "Texture.h"
#include "Player.h"
#include "Utils.h"
#include "glm/glm.hpp"
#include <glm/ext/quaternion_transform.hpp>
#include "SFML/Graphics.hpp"
#include "Game.h"

sf::VertexArray Graphics::verticles = sf::VertexArray(sf::Quads, Graphics::instances.size() * 4);
Texture* Graphics::spriteSheet; 
sf::RenderStates* Graphics::states;
std::vector<int> Graphics::indexes;
std::vector<glm::vec3> Graphics::localPositions;
Event<Graphics*> Graphics::onSpawn;
Event<Graphics*> Graphics::onDestroy;
std::vector<Graphics*> Graphics::instances;
std::vector<Graphics*> Graphics::destroyMarkeds;
glm::vec2 Graphics::tl = { -1, -1 };
glm::vec2 Graphics::tr = { 1,-1 };
glm::vec2 Graphics::br = { 1,1 };
glm::vec2 Graphics::bl = { -1,1 };

Graphics::Graphics()
{
	instances.push_back(this);
	onSpawn.Invoke(this);
}

Graphics::~Graphics()
{
	onDestroy.Invoke(this);
	remove(instances, this);
}

void Graphics::onOverlap(Graphics* other)
{

}

bool Graphics::isActiveCollider()
{
	return false;
}

void Graphics::Destroy()
{
	if (indexOf(destroyMarkeds, this) < 0) {
		destroyMarkeds.push_back(this);
	}
}

void Graphics::init()
{
	spriteSheet = new Texture("img/spriteSheet.png");
	states = new sf::RenderStates();
	states->texture = spriteSheet;
	states->transform.translate({ Config::SCREEN_WIDTH / 2.f, Config::SCREEN_HEIGHT / 2.f });
}

void Graphics::deleteDestroyMarkeds()
{
	for (auto marked : destroyMarkeds) {
		delete(marked);
	}

	destroyMarkeds.clear();
}

void Graphics::updateVerticles()
{
    for (int p = 0; p < indexes.size(); ++p) {
        int vi = p * 4;

        if (localPositions[indexes[p]].z <= 1.) {
            verticles[vi].position = {};
            verticles[vi + 1].position = {};
            verticles[vi + 2].position = {};
            verticles[vi + 3].position = {};
            continue;
        }

        glm::vec3 screenPos = Player::main->local2ScreenPos(localPositions[indexes[p]]);

        float depthScale = std::clamp(1. / (glm::length(localPositions[indexes[p]]) / Config::DEPTH_SCALE), 0., 1.);
        float size = Graphics::instances[indexes[p]]->size * depthScale * 0.5;
        float rotRad = glm::radians(Graphics::instances[indexes[p]]->rotation);

        verticles[vi].position = v2f(screenPos + rotate(tl * size, rotRad));
        verticles[vi + 1].position = v2f(screenPos + rotate(tr * size, rotRad));
        verticles[vi + 2].position =  v2f(screenPos + rotate(br * size, rotRad));
        verticles[vi + 3].position = v2f(screenPos + rotate(bl * size, rotRad));

        float shade = 255 * (1 - glm::pow(1 - depthScale, Config::LIGHT_FALL_RATE));
        sf::Color color = sf::Color(shade, shade, shade);
        verticles[vi].color = color;
        verticles[vi + 1].color = color;
        verticles[vi + 2].color = color;
        verticles[vi + 3].color = color;
    }
}

void Graphics::fillTextureCoords()
{
	sf::Vector2f texSize = static_cast<sf::Vector2f>(spriteSheet->getSize());
	glm::vec4 uvs;
	glm::vec4 pixCoords;

	for (int i = 0; i < indexes.size(); ++i) {
		int si = i * 4;

		uvs = Graphics::instances[indexes[i]]->getUVs();
		pixCoords = { uvs.x * texSize.x, uvs.y * texSize.y, uvs.z * texSize.x, uvs.w * texSize.y };

		verticles[si].texCoords = { pixCoords.x, pixCoords.y };
		verticles[si + 1].texCoords = { pixCoords.z, pixCoords.y };
		verticles[si + 2].texCoords = { pixCoords.z, pixCoords.w };
		verticles[si + 3].texCoords = { pixCoords.x , pixCoords.w };
	}
}

void Graphics::updateLocalPositions()
{
	for (int p = 0; p < localPositions.size(); p++) {
		localPositions[p] = Player::main->world2LocalPos(Graphics::instances[p]->position);
	}
}

void Graphics::updateCollisions()
{
	Graphics* instance;
	Graphics* other;
	for (int i = 0; i < indexes.size(); i++) {
		instance = Graphics::instances[i];
		if (!instance->isActiveCollider())continue;

		for (int j = 0; j < Graphics::instances.size(); j++) {
			if (i == j)continue;
			if (i >= localPositions.size() || j >= localPositions.size())continue;

			other = Graphics::instances[j];
			if (isOverlaps(instance->size, other->size, localPositions[j], localPositions[i])) {
				instance->onOverlap(other);
				if (!other->isActiveCollider()) {
					other->onOverlap(instance);
				}
			}
		}
	}
}

bool Graphics::isOverlaps(const float sizeA, const float sizeB, const glm::vec3 posA, const glm::vec3 posB)
{
	float minDist = (sizeA + sizeB) * 0.5;
	glm::vec3 delta = abs(posB - posA);

	if (delta.x > minDist || delta.y > minDist || delta.z > minDist)return false;

	return glm::length(delta) < minDist;
}

void Graphics::sortByLocalZ() {
	std::sort(indexes.begin(), indexes.end(), [&](const int a, const int b) {
		if (a >= localPositions.size()) {
			std::cout << "Error: " << std::to_string(a) << " is out of range " << std::to_string(localPositions.size()) << std::endl;
			return false;
		}

		if (b >= localPositions.size()) {
			std::cout << "Error: " << std::to_string(b) << " is out of range " << std::to_string(localPositions.size()) << std::endl;
			return false;
		}

		return localPositions[a].z > localPositions[b].z;
		});
}

void Graphics::updateGraphicsBehaviours() {
	for (int i = Graphics::instances.size() - 1; i > -1; --i) {
		Graphics::instances[i]->update();
	}
}

void Graphics::fixVectorSizes() {
	if (indexes.size() != Graphics::instances.size()) {
		indexes.resize(Graphics::instances.size());
	}
	for (int i = 0; i < indexes.size(); i++) {
		indexes[i] = i;
	}

	if (localPositions.size() != Graphics::instances.size()) {
		localPositions.resize(Graphics::instances.size());
	}

	if (verticles.getVertexCount() != Graphics::instances.size() * 4) {
		verticles.resize(Graphics::instances.size() * 4);
	}
}

void Graphics::draw()
{
	Game::window->draw(verticles, *states);
}

void Graphics::updateBehaviours()
{
	deleteDestroyMarkeds();
	fixVectorSizes();
	updateGraphicsBehaviours();
	updateLocalPositions();
	updateCollisions();
	sortByLocalZ();
	updateVerticles();
	fillTextureCoords();
}
