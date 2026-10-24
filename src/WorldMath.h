#pragma once
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Config.h"

inline glm::vec3 wrappedDelta(const glm::vec3& delta)
{
    return delta - Config::FIELD_SIZE * glm::floor((delta + Config::HALF_FIELD_SIZE) / Config::FIELD_SIZE);
}

inline glm::vec3 cameraLocalPosition(const glm::vec3& world, const glm::vec3& camera, const glm::quat& rotation)
{
    return glm::inverse(rotation) * wrappedDelta(world - camera);
}

inline glm::vec3 projectToScreen(const glm::vec3& local)
{
    const float depth = glm::max(local.z, 1.0f);
    // Preserve the original learning project's projection scale. This angle is not a calibrated horizontal FOV.
    const float scale = depth / std::tan(glm::radians(Config::CAMERA_PROJECTION_ANGLE) / 2.0f);
    return {local.x / scale * 0.5f * Config::SCREEN_WIDTH,
            local.y / scale * 0.5f * Config::SCREEN_HEIGHT, scale};
}
