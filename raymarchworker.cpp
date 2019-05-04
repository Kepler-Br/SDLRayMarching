#include "raymarchworker.h"
#include <iostream>

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

    const int maxSteps = 50;
    const float maxDistance = 10.0f;
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

glm::vec3 RayMarchWorker::screenToWorld(const glm::ivec2 &screenPosition, const glm::ivec2 &renderViewportSize, const glm::mat4 &inverseVP)
{
    // NORMALISED DEVICE SPACE
    float x = 2.0f * screenPosition.x / renderViewportSize.x - 1.0f;
    float y = 2.0f * screenPosition.y / renderViewportSize.y - 1.0f;

    // HOMOGENEOUS SPACE
    glm::vec4 screenPos = glm::vec4(x, -y, -1.0f, 1.0f);

    glm::vec4 worldPos = inverseVP * screenPos;
    return glm::vec3(worldPos);
}

void RayMarchWorker::threadFunction()
{
    while (isRunning)
    {
        getJobMutex.lock();
        const RayMarchJob job = getJob();
        getJobMutex.unlock();

        // Wait untill new jobs available
        if(!job.newJobAvailable)
        {
            workDone = true;
            std::this_thread::sleep_for(std::chrono::microseconds(10));
            continue;
        }
        // If new jobs generation is ended.
        if(job.isShuttingDown)
        {
            workDone = true;
            isRunning = false;
            continue;
        }
        workDone = false;
        for(uint i = 0; i < job.pixelsToRender; i++)
        {
            uint currentPixel = job.pixelNumber+i;
            const uint pixelX = currentPixel%static_cast<uint>(renderViewportSize.x);
            const uint pixelY = currentPixel/static_cast<uint>(renderViewportSize.x);
            glm::ivec2 pixelPosition = glm::ivec2(pixelX, pixelY);
            glm::vec3 rayOrigin = cameraPosition;
            glm::vec3 rayDirection = RayMarchWorker::screenToWorld(pixelPosition, renderViewportSize, inverseVP);
            float distance = RayMarchWorker::rayMarch(rayOrigin, rayDirection)/100.0f;
            distance = distance > 1.0f?1.0f:distance;
            glm::vec3 color(distance);
//            glm::vec3 color(1.0f);
            getJobMutex.lock();
            pixels->operator[](currentPixel) = color;
            getJobMutex.unlock();
        }
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

bool RayMarchWorker::isWorkDone() const
{
    return workDone;
}

void RayMarchWorker::setCameraMatrices(const glm::mat4 &view, const glm::mat4 &projection, const glm::vec3 &position)
{
    this->view = view;
    this->projection = projection;
    cameraPosition = position;
    updateMatrices();
}

void RayMarchWorker::updateMatrices()
{
    VP = projection*view;
    inverseVP = inverse(VP);
}
