#pragma once

#include "SFML/Graphics.hpp"
#include "glm/glm.hpp"
#include "Event.h"
#include "Texture.h"

class Graphics
{
	static Texture* spriteSheet;
	static std::vector<int> indexes;
	static std::vector<glm::vec3> localPositions;
	static glm::vec2 tl;
	static glm::vec2 tr;
	static glm::vec2 br;
	static glm::vec2 bl;
	static sf::VertexArray verticles;
	static sf::RenderStates* states;

	static void deleteDestroyMarkeds();

	static void updateVerticles();

	static void fillTextureCoords();

	static void updateLocalPositions();

	static void updateCollisions();

	static bool isOverlaps(const float sizeA, const float sizeB, const glm::vec3 posA, const glm::vec3 posB);

	static void sortByLocalZ();

	static void updateGraphicsBehaviours();

	static void fixVectorSizes();

public:
	static Event<Graphics*> onSpawn;
	static Event<Graphics*> onDestroy;
	static std::vector<Graphics*> instances;
	static std::vector<Graphics*> destroyMarkeds;

	Graphics();
	virtual ~Graphics();

	glm::vec3 position = {0.,0.,0.};
	float size = 1;
	float rotation = 0;
	std::string name = "unnamed";

	inline virtual glm::vec4 getUVs() const {
		return { 0,0,1,1 };
	};

	virtual void update(){};

	virtual void onOverlap(Graphics* other);

	virtual bool isActiveCollider();

	void Destroy();

	static void init();

	static void draw();

	static void updateBehaviours();
};

