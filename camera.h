#ifndef CAMERA_H
#define CAMERA_H
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

class Camera
{
    glm::mat4 projection;
    glm::mat4 view;
    glm::ivec2 screenGeometry;
    glm::vec3 position;

public:
    Camera(glm::ivec2 screenGeometry);
    glm::vec3 getPosition();
    glm::vec3 screenToWorld(glm::ivec2 screenPosition);
    glm::mat4 getView();
    glm::mat4 getProjection();
};

#endif // CAMERA_H
