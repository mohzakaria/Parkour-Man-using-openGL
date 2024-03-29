#pragma once
#include <iostream>
#include "../ecs/world.hpp"
#include "../components/camera.hpp"
#include "../components/free-camera-controller.hpp"
#include "../material/material.hpp"
#include "../application.hpp"
// #include "../components/player.hpp"
#include "../components/movement.hpp"
#include "../systems/collision.hpp"
#include "../components/player.hpp"
#include "../systems/forward-renderer.hpp"
// #include "../systems/forward-renderer.cpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include "../systems/forward-renderer.hpp"
#define postProcessingDuration 20
//
namespace our
{

    // The free camera controller system is responsible for moving every entity which contains a FreeCameraControllerComponent.
    // This system is added as a slightly complex example for how use the ECS framework to implement logic.
    // For more information, see "common/components/free-camera-controller.hpp"
    class FreeCameraControllerSystem
    {
        Application *app;          // The application in which the state runs
        bool mouse_locked = false; // Is the mouse locked
        bool isColliding = false;
        bool isAboveBlock = false;
        float endpositiony = 0.0f;
        bool isCollidingFront = false;
        bool isCollidingBack = false;
        bool isCollidingLeft = false;
        bool isCollidingRight = false;
        bool isBelowBlock = false;
        CollisionComponent *tmpi = nullptr;
        Material *material = nullptr;
        our::ForwardRenderer *forwardRenderer;
        int postProcessTestCounter;

    public:
        bool applyingPostProcess;     // whether we are applying post processing or not
        bool testApplyingPostProcess; // whether we are applying post processing or not
        int postProcessingCurrentCounter;
        // When a state enters, it should call this function and give it the pointer to the application
        void enter(Application *app, our::ForwardRenderer *forwardRenderer)
        {
            postProcessTestCounter = 0;
            this->app = app;
            this->forwardRenderer = forwardRenderer;
        }
        void checkForPostProcessing()
        {
            // applying post processing effect for some duration defined by postProcessingDuration
            if (applyingPostProcess)
            {
                // if the duration is reached it will terminate the post processing effect
                if (postProcessingCurrentCounter == postProcessingDuration)
                {
                    // change post processing effect to default
                    forwardRenderer->changePostprocessShader(0);
                    applyingPostProcess = false;
                    // reset the frames counter
                    postProcessingCurrentCounter = 0;
                }
                else
                {
                    // increment the frames counter
                    postProcessingCurrentCounter++;
                }
            }
        }

        // This should be called every frame to update all entities containing a FreeCameraControllerComponent
        void update(World *world, float deltaTime)
        {

            // First of all, we search for an entity containing both a CameraComponent and a FreeCameraControllerComponent
            // As soon as we find one, we break
            CameraComponent *camera = nullptr;
            FreeCameraControllerComponent *controller = nullptr;
            // CollisionSystem *collision = nullptr;
            PlayerComponent *player = nullptr;

            for (auto entity : world->getEntities())
            {
                camera = entity->getComponent<CameraComponent>();
                controller = entity->getComponent<FreeCameraControllerComponent>();
                // player = entity->getComponent<PlayerComponent>(); // create player component
                // movement = entity->getComponent<MovementComponent>();
                if (camera && controller)
                    break;
            }
            // If there is no entity with both a CameraComponent and a FreeCameraControllerComponent, we can do nothing so we return
            if (!(camera && controller))
                return;
            // Get the entity that we found via getOwner of camera (we could use controller->getOwner())
            Entity *entity = camera->getOwner();

            // If the left mouse button is pressed, we lock and hide the mouse. This common in First Person Games.
            if (app->getMouse().isPressed(GLFW_MOUSE_BUTTON_1) && !mouse_locked)
            {
                app->getMouse().lockMouse(app->getWindow());
                mouse_locked = true;
                // If the left mouse button is released, we unlock and unhide the mouse.
            }
            else if (!app->getMouse().isPressed(GLFW_MOUSE_BUTTON_1) && mouse_locked)
            {
                app->getMouse().unlockMouse(app->getWindow());
                mouse_locked = false;
            }

            // We get a reference to the entity's position and rotation
            glm::vec3 &position = entity->localTransform.position;
            glm::vec3 &rotation = entity->localTransform.rotation;
            bool isPlayerJumping = controller->isJumping;
            bool isPlayerFalling = controller->isFalling; // AAO

            // If the left mouse button is pressed, we get the change in the mouse location
            // and use it to update the camera rotation
            // if (app->getMouse().isPressed(GLFW_MOUSE_BUTTON_1))
            // {
            //     glm::vec2 delta = app->getMouse().getMouseDelta();
            //     rotation.x -= delta.y * controller->rotationSensitivity; // The y-axis controls the pitch
            //     rotation.y -= delta.x * controller->rotationSensitivity; // The x-axis controls the yaw
            // }

            // We prevent the pitch from exceeding a certain angle from the XZ plane to prevent gimbal locks
            if (rotation.x < -glm::half_pi<float>() * 0.99f)
                rotation.x = -glm::half_pi<float>() * 0.99f;
            if (rotation.x > glm::half_pi<float>() * 0.99f)
                rotation.x = glm::half_pi<float>() * 0.99f;
            // This is not necessary, but whenever the rotation goes outside the 0 to 2*PI range, we wrap it back inside.
            // This could prevent floating point error if the player rotates in single direction for an extremely long time.
            rotation.y = glm::wrapAngle(rotation.y);

            // We update the camera fov based on the mouse wheel scrolling amount
            float fov = camera->fovY + app->getMouse().getScrollOffset().y * controller->fovSensitivity;
            fov = glm::clamp(fov, glm::pi<float>() * 0.01f, glm::pi<float>() * 0.99f); // We keep the fov in the range 0.01*PI to 0.99*PI
            camera->fovY = fov;

            // We get the camera model matrix (relative to its parent) to compute the front, up and right directions
            glm::mat4 matrix = entity->localTransform.toMat4();

            glm::vec3 front = glm::vec3(matrix * glm::vec4(0, 0, -1, 0)),
                      up = glm::vec3(matrix * glm::vec4(0, 26, 0, 0)),
                      right = glm::vec3(matrix * glm::vec4(1, 0, 0, 0)),
                      forward = glm::vec3(matrix * glm::vec4(0, 0, -3, 0));

            glm::vec3 current_sensitivity = controller->positionSensitivity;
            glm::vec3 down = glm::vec3(matrix * glm::vec4(0, -1, 0, 0)); // -1.0f MESH -2.0f
            // If the LEFT SHIFT key is pressed, we multiply the position sensitivity by the speed up factor
            if (app->getKeyboard().isPressed(GLFW_KEY_LEFT_SHIFT))
                current_sensitivity *= controller->speedupFactor;

            // We change the camera position based on the keys WASD/QE
            // S & W moves the player back and forth
            if (app->getKeyboard().isPressed(GLFW_KEY_UP) && !isCollidingFront)
                position += front * (deltaTime * current_sensitivity.z);
            if (app->getKeyboard().isPressed(GLFW_KEY_DOWN) && !isCollidingBack)
                position -= front * (deltaTime * current_sensitivity.z);
            // A & D moves the player left or right
            if (app->getKeyboard().isPressed(GLFW_KEY_RIGHT) && !isCollidingRight)
                position += right * (deltaTime * current_sensitivity.x);
            if (app->getKeyboard().isPressed(GLFW_KEY_LEFT) && !isCollidingLeft)
                position -= right * (deltaTime * current_sensitivity.x);
            // Q & E moves the player up and down
            // AAO
            CollisionComponent *jake = nullptr;
            for (auto entity : world->getEntities())
            {
                if (entity->getComponent<PlayerComponent>())
                {
                    player = entity->getComponent<PlayerComponent>();
                    jake = entity->getComponent<CollisionComponent>();
                    break;
                }
            }

            for (auto entity : world->getEntities())
            {

                PlayerComponent *player = entity->getComponent<PlayerComponent>();
                if (player)
                {

                    continue;
                }

                glm::vec3 jakeAbsPosition = position + glm::vec3(0.0f, -1.0f, -1.1f);
                if (jakeAbsPosition.y <= -1.0f)
                {
                    forwardRenderer->changePostprocessShader(1);
                    app->changeState("gameover");
                }
                if (jakeAbsPosition.z <= -23)
                {

                    forwardRenderer->changePostprocessShader(3);
                    if (jakeAbsPosition.z <= -85)
                    {
                        app->changeState("win");
                    }
                    else
                    {
                        position += front * (deltaTime * current_sensitivity.z);
                    }
                }
                CollisionComponent *tmpcol = entity->getComponent<CollisionComponent>();
                if (tmpcol)
                {

                    isColliding = tmpcol->isColliding((tmpcol->start), (tmpcol->end), (jake->start + position), (jake->end + position));
                    if (isColliding)
                    {

                        tmpi = tmpcol;

                        isAboveBlock = jakeAbsPosition.y > tmpcol->end.y;

                        if (!isAboveBlock)
                        {
                            isCollidingFront = abs(jakeAbsPosition.z) >= abs(tmpcol->start.z) && abs(jakeAbsPosition.z) < abs(tmpcol->start.z) + 0.1;
                            isCollidingBack = abs(jakeAbsPosition.z) <= abs(tmpcol->end.z) && abs(jakeAbsPosition.z) > abs(tmpcol->end.z) - 0.1;
                            isCollidingLeft = jakeAbsPosition.x >= tmpcol->end.x;
                            isCollidingRight = jakeAbsPosition.x <= tmpcol->start.x;
                        }
                        isBelowBlock = jakeAbsPosition.y <= tmpcol->start.y;
                        
                    }

                    if (tmpi == tmpcol && !isColliding)
                    {
                        isAboveBlock = false;
                        isCollidingFront = false;
                        isCollidingBack = false;
                        isCollidingLeft = false;
                        isCollidingRight = false;
                        isBelowBlock = false;
                    }
                }
            }
            if (app->getKeyboard().isPressed(GLFW_KEY_SPACE) && !isPlayerJumping && !isPlayerFalling)
            {
                controller->isJumping = true;
                if (!isPlayerFalling)
                {
                    controller->position = position;
                }
            }
            if (isPlayerJumping)
            {
                if (position.y <= (controller->position).y + 3.0f && (!isBelowBlock))
                {
                    position += up * (deltaTime * current_sensitivity.y * 0.1f);
                }
                else
                {
                    controller->isJumping = false;
                    controller->isFalling = true;
                }
            }
            if (isPlayerFalling)
            {

                if (position.y >= 0.0f && (!isAboveBlock))
                {
                    position += down * (deltaTime * current_sensitivity.y);
                }
                else
                {
                    controller->isFalling = false;
                }
            }
            if (!isAboveBlock && !isPlayerJumping && !isPlayerFalling)
            {
                controller->isFalling = true;
            }
        }

        // When the state exits, it should call this function to ensure the mouse is unlocked
        void exit()
        {
            
            if (mouse_locked)
            {
                mouse_locked = false;
                app->getMouse().unlockMouse(app->getWindow());
            }
        }
    };

}