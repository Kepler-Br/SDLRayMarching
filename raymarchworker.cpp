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

        // Wait untill new jobs available
        if(pixelPosition.x == -1)
        {
            std::this_thread::sleep_for(std::chrono::microseconds(10));
            continue;
        }
        // If new jobs generation is ended.
        if(pixelPosition.x == -2)
        {
            isRunning = false;
            continue;
        }
        glm::vec3 rayOrigin = camera.getPosition();
        glm::vec3 rayDirection = camera.screenToWorld(pixelPosition);
        float distance = rayMarch(rayOrigin, rayDirection)/50.0f;
        distance = distance > 1.0f?1.0f:distance;
        glm::vec3 color(distance);

        setPixelMutex.lock();
        window.setPixel(pixelPosition, color);
        setPixelMutex.unlock();
    }
}

RayMarchWorker::~RayMarchWorker()
{
    if(thread != nullptr)
    {
        thread->join();
        delete thread;
    }
}

void RayMarchWorker::run()
{
    isRunning = true;
    thread = new std::thread(std::bind(&RayMarchWorker::threadFunction, this));
}
