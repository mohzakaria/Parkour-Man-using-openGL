#pragma once

#include "../ecs/component.hpp"

#include <glm/glm.hpp>
#include <json/json.hpp>
namespace our
{
    class PlayerComponent : public Component
    {
    public:
        int speed;
        static std::string getID() { return "Player"; }

        void deserialize(const nlohmann::json &data) override;
    };

}