#pragma once

#include "../components/collision.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

namespace our
{

    // The movement system is responsible for moving every entity which contains a MovementComponent.
    // This system is added as a simple example for how use the ECS framework to implement logic.
    // For more information, see "common/components/movement.hpp"
    class CollisionSystem
    {
    public:
        // bool isColliding(glm::vec3 postionA, glm::vec3 postionB, glm::vec3 sizeA, glm::vec3 sizeB)
        // {
        //     if (postionA.x + sizeA.x >= postionB.x &&
        //         postionB.x + sizeB.x >= postionA.x &&
        //         postionA.y + sizeA.y >= postionB.y &&
        //         postionB.y + sizeB.y >= postionA.y &&
        //         postionA.z + sizeA.z >= postionB.z &&
        //         postionB.z + sizeB.z >= postionA.z)
        //     {
        //         return true;
        //     }
        //     return false;
        // }
        bool isColliding(glm::vec3 startA, glm::vec3 endA, glm::vec3 startB, glm::vec3 endB)
        {
            if (startA.x <= endB.x &&
                endA.x >= startB.x &&
                startA.y <= endB.y &&
                endA.y >= startB.y &&
                startA.z <= endB.z &&
                endA.z >= startB.z)
            {
                return true;
            }
            return false;
        }
    };

}
