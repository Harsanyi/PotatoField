#pragma once
#include <memory>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include "Event.h"
#include "Texture.h"

class Graphics
{
    static std::unique_ptr<Texture> spriteSheet;
    static std::vector<std::size_t> indexes;
    static std::vector<glm::vec3> localPositions;
    static sf::VertexArray vertices;
    static sf::RenderStates states;
    static std::vector<Graphics*> markedForDestruction;
    static bool shuttingDown;
    bool pendingDestruction = false;

    static void deleteMarkedForDestruction();
    static void updateVertices();
    static void fillTextureCoords();
    static void updateLocalPositions();
    static void updateCollisions();
    static bool overlaps(const Graphics& first, const Graphics& second);
    static void sortByLocalZ();
    static void updateGraphicsBehaviours();
    static void fixVectorSizes();

public:
    static Event<Graphics*> onSpawn;
    static Event<Graphics*> onDestroy;
    // This registry owns heap-allocated world objects. External raw pointers are observers.
    static std::vector<Graphics*> instances;

    Graphics();
    virtual ~Graphics();
    Graphics(const Graphics&) = delete;
    Graphics& operator=(const Graphics&) = delete;

    glm::vec3 position{0};
    float size = 1;
    float rotation = 0;
    std::string name = "unnamed";
    virtual glm::vec4 getUVs() const { return {0, 0, 1, 1}; }
    virtual void update() {}
    virtual void onOverlap(Graphics*) {}
    virtual bool isActiveCollider() { return false; }
    void Destroy();
    bool isPendingDestruction() const { return pendingDestruction; }

    static void init();
    static void shutdown();
    static bool isShuttingDown() { return shuttingDown; }
    static void draw();
    static void updateBehaviours();
};
