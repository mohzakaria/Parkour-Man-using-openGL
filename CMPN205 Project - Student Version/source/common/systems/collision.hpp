#pragma once

#include "../ecs/component.hpp"

#include <glm/glm.hpp>

namespace our
{

    // This component denotes that the MovementSystem will move the owning entity by a certain linear and angular velocity.
    // This component is added as a simple example for how use the ECS framework to implement logic.
    // For more information, see "common/systems/movement.hpp"
    // For a more complex example of how to use the ECS framework, see "free-camera-controller.hpp"
    class CollisionComponent : public Component
    {
    public:
        glm::vec3 start, end;

        // The ID of this component type is "Movement"
        static std::string getID() { return "Collision"; }

        // Reads linearVelocity & angularVelocity from the given json object
        void deserialize(const nlohmann::json &data) override;
        bool isColliding(glm::vec3 startA, glm::vec3 endA, glm::vec3 startB, glm::vec3 endB)
        {
            if (startA.x <= endB.x &&
                endA.x >= startB.x &&
                startA.y <= endB.y &&
                endA.y >= startB.y &&
                abs(startA.z) <= abs(endB.z) &&
                abs(endA.z) >= abs(startB.z))
            {
                return true;
            }
            return false;
        }
    };

}
