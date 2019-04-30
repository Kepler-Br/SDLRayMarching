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
        const glm::ivec2 job = jobFunction();
        getJobMutex.unlock();



        // Wait untill new jobs available
        if(job.x == -1)
        {
            workDone = true;
            std::this_thread::sleep_for(std::chrono::microseconds(10));
            continue;
        }
//         If new jobs generation is ended.
        if(job.x == -2)
        {
            workDone = true;
            isRunning = false;
            continue;
        }
        if(job.y > donePixels.size())
            donePixels.resize((uint)job.y, glm::vec3(1.0f));
        workDone = false;
        for(int i = 0; i < (int)donePixels.size(); i++)
        {
            int currentPixel = job.x+i;
            glm::ivec2 pixelPosition = glm::ivec2(currentPixel%);
            glm::vec3 rayOrigin = camera.getPosition();
            glm::vec3 rayDirection = camera.screenToWorld(pixelPosition);
            float distance = rayMarch(rayOrigin, rayDirection)/100.0f;
            distance = distance > 1.0f?1.0f:distance;
            glm::vec3 color(1.0f);
        }

//        const glm::ivec2 &windowGeometry = window.getGeometry();
        setPixelMutex.lock();

        for(int i = 0; i < (int)donePixels.size(); i++)
        {
            int currentPixel = job.x+i;
            const glm::vec3 &color = donePixels[i];

            pixels[currentPixel] = color;
        }
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
