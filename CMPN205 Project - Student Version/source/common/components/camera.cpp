#include "camera.hpp"
#include "../ecs/entity.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 

namespace our {
    // Reads camera parameters from the given json object
    void CameraComponent::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;
        std::string cameraTypeStr = data.value("cameraType", "perspective");
        if(cameraTypeStr == "orthographic"){
            cameraType = CameraType::ORTHOGRAPHIC;
        } else {
            cameraType = CameraType::PERSPECTIVE;
        }
        near = data.value("near", 0.01f);
        far = data.value("far", 100.0f);
        fovY = data.value("fovY", 90.0f) * (glm::pi<float>() / 180);
        orthoHeight = data.value("orthoHeight", 1.0f);
    }

    // Creates and returns the camera view matrix
    glm::mat4 CameraComponent::getViewMatrix() const {
        auto owner = getOwner();
        auto M = owner->getLocalToWorldMatrix();
        //TOD: (Req 8) Complete this function
        //HINT:
        // In the camera space:
        // - eye is the origin (0,0,0)
        // - center is any point on the line of sight. So center can be any point (0,0,z) where z < 0. For simplicity, we let center be (0,0,-1)
        // - up is the direction (0,1,0)
        // but to use glm::lookAt, we need eye, center and up in the world state.
        // Since M (see above) transforms from the camera to thw world space, you can use M to compute:
        // - the eye position which is the point (0,0,0) but after being transformed by M
        // - the center position which is the point (0,0,-1) but after being transformed by M
        // - the up direction which is the vector (0,1,0) but after being transformed by M
        // then you can use glm::lookAt
        
        /*
        First glm::lookAt takes the vectors as vec3 not vec4 but the transforamtion matrix is vec4 so
        we will need to convert the vectors and points after we transform them to vec3
        test 
        float or double?
        */
       glm::vec3 eyePosition = glm::vec3(M * glm::vec4(0,0,0,1));
       glm::vec3 centerPosition = glm::vec3(M * glm::vec4(0,0,-1,1));
       glm::vec3 upVector = glm::vec3(M * glm::vec4(0,1,0,0));
       return glm::lookAt(eyePosition, centerPosition, upVector);

        // return glm::mat4(1.0f);
    }

    // Creates and returns the camera projection matrix
    // "viewportSize" is used to compute the aspect ratio
    glm::mat4 CameraComponent::getProjectionMatrix(glm::ivec2 viewportSize) const {
        //TOD: (Req 8) Wrtie this function
        // NOTE: The function glm::ortho can be used to create the orthographic projection matrix
        // It takes left, right, bottom, top. Bottom is -orthoHeight/2 and Top is orthoHeight/2.
        // Left and Right are the same but after being multiplied by the aspect ratio
        // For the perspective camera, you can use glm::perspective
        
        /*
        First we check if the camera is orthographic or perspective
        Then we create the projection matrix based on the camera type
        also we precaculate the aspect ratio to use it in the orthographic camera
        and theperspective camera
        finally we return the projection matrix
        test
        float or double?
        Radian wla la2?
        */
        glm::mat4 projectionMatrix;
        float aspectRatio = ((float)viewportSize.x / viewportSize.y);
        if (cameraType == CameraType::ORTHOGRAPHIC)
        {
            float left = ((-orthoHeight / 2) * aspectRatio);
            float right = ((orthoHeight / 2) * aspectRatio);
            float bottom = (-orthoHeight / 2);
            float top = (orthoHeight / 2);
            projectionMatrix = glm::ortho(left, right, bottom, top, near, far);
        }
        else
        {
            projectionMatrix = glm::perspective(fovY, aspectRatio,near, far);
        }
        return projectionMatrix;
        
        //return glm::mat4(1.0f);
    }
}