#include "raymarchworker.h"

float RayMarchWorker::getDistance(glm::vec3 currentMarchingLocation)
{
    glm::vec3 sphere = glm::vec3(0.0f, 1.0f, 6.0f);
    float sphereRadius = 1.0f;
    float sphereDist = glm::length(currentMarchingLocation-sphere)-sphereRadius;

    float planeDistance = currentMarchingLocation.y;

    float distance = glm::min(sphereDist, planeDistance);
    return distance;

}

float RayMarchWorker::rayMarch(glm::vec3 &rayOrigin, glm::vec3 &rayDirection)
{
    float distanceFromOrigin = 0.0f;

    const int maxSteps = 100;
    const float maxDistance = 100.0f;
    const float minDistanceToSurface = 0.01f;

    for(int i = 0; i < maxSteps; i++)
    {
        glm::vec3 currentMarchingLocation = rayOrigin + rayDirection * distanceFromOrigin;
        float distanceToScene = getDistance(currentMarchingLocation);
        distanceFromOrigin += distanceToScene;
        if(distanceFromOrigin < minDistanceToSurface ||
                distanceFromOrigin > maxDistance)
            break;
    }

    return distanceFromOrigin;
}

void RayMarchWorker::threadFunction()
{
    while (isRunning)
    {
        getJobMutex.lock();
        glm::ivec2 pixelPosition = jobFunction();
        getJobMutex.unlock();

        if(pixelPosition.x == -1)
            isRunning = true;
        if(pixelPosition.x == -2)
            isRunning = false;



        glm::vec3 rayOrigin = camera.getPosition();
        glm::vec3 rayDirection = camera.screenToWorld(pixelPosition);
        float distance = rayMarch(rayOrigin, rayDirection);
        distance = distance > 1.0f?1.0f:distance;
        glm::vec3 color(distance);
        window.setPixel(pixelPosition, color);

    }
}

RayMarchWorker::~RayMarchWorker()
{
    thread.join();
}

void RayMarchWorker::run()
{
    thread = std::thread(std::bind(&RayMarchWorker::threadFunction, this));
    threadFunction();
}
