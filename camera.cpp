#include "camera.h"

Camera::Camera(glm::ivec2 screenGeometry):
    screenGeometry(screenGeometry)
{
    const float fov = 3.14f/4.0f;
    const float aspect = float(screenGeometry.x)/screenGeometry.y;
    const float near = 20.0f;
    const float far = 1000.0f;
    projection = glm::perspective(fov, aspect, near, far);
    position = glm::vec3(0.0f, 0.2123f, 0.0f);

    view = glm::lookAt(
                position, // Camera location
                glm::vec3(0.0f, 0.0f, 1.0f), // Where camera is looking
                glm::vec3(0.0f, 0.0f, 1.0f) // Camera head is facing up
                );

    std::cout << glm::to_string(view) << std::endl;
    std::cout << glm::to_string(projection) << std::endl;
}

glm::vec3 Camera::getPosition()
{
    return position;
}

glm::vec3 Camera::screenToWorld(glm::ivec2 screenPosition)
{
    // NORMALISED DEVICE SPACE
    double x = 2.0 * screenPosition.x / screenGeometry.x - 1;
    double y = 2.0 * screenPosition.y / screenGeometry.y - 1;

    // HOMOGENEOUS SPACE
    glm::vec4 screenPos = glm::vec4(x, -y, -1.0f, 1.0f);

    // Projection/Eye Space
    glm::mat4 projectView = projection * view;
    glm::mat4 viewProjectionInverse = inverse(projectView);

    glm::vec4 worldPos = viewProjectionInverse * screenPos;
    return glm::vec3(worldPos);
}

glm::mat4 Camera::getView()
{
    return view;
}

glm::mat4 Camera::getProjection()
{
    return projection;
}
