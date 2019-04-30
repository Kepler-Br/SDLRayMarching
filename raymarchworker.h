#ifndef RAYMARCHWORKER_H
#define RAYMARCHWORKER_H

#include <functional>
#include <glm/glm.hpp>
#include <mutex>
#include <thread>
#include <vector>

#include "camera.h"
#include "window.h"

class RayMarchWorker
{
    std::function<glm::ivec2()> jobFunction;
    std::mutex &getJobMutex;
    std::mutex &setPixelMutex;
    std::vector<glm::vec3> &pixels;
    Camera &camera;
    int totalWorkers;
    std::vector<glm::vec3> donePixels;
    bool workDone = true;

    std::thread *thread = nullptr;

    bool isRunning = false;

    float getDistance(glm::vec3 currentMarchingLocation);
    float rayMarch(glm::vec3 &rayOrigin, glm::vec3 &rayDirection);
    void threadFunction();

public:
    RayMarchWorker(std::function<glm::ivec2()> jobFunction, std::mutex &getJobMutex, std::mutex &setPixelMutex, std::vector<glm::vec3> &pixels, Camera &camera, int totalWorkers):
        jobFunction(jobFunction),
        getJobMutex(getJobMutex),
        setPixelMutex(setPixelMutex),
        pixels(pixels),
        camera(camera),
        totalWorkers(totalWorkers)
    {

    }

    ~RayMarchWorker();

    void run();

    bool isWorkDone()
    {
        return workDone;
    }
};

#endif // RAYMARCHWORKER_H
