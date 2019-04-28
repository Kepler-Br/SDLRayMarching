#ifndef RAYMARCHWORKER_H
#define RAYMARCHWORKER_H

#include <functional>
#include <glm/glm.hpp>
#include <mutex>
#include <thread>

#include "camera.h"
#include "window.h"

class RayMarchWorker
{
    std::function<glm::ivec2()> jobFunction;
    std::mutex &getJobMutex;
    std::mutex &setPixelMutex;
    Window &window;
    Camera &camera;

    std::thread *thread = nullptr;

    bool isRunning = false;

    float getDistance(glm::vec3 currentMarchingLocation);
    float rayMarch(glm::vec3 &rayOrigin, glm::vec3 &rayDirection);
    void threadFunction();

public:
    RayMarchWorker(std::function<glm::ivec2()> jobFunction, std::mutex &getJobMutex, std::mutex &setPixelMutex, Window &window, Camera &camera):
        jobFunction(jobFunction),
        setPixelMutex(setPixelMutex),
        getJobMutex(getJobMutex),
        window(window),
        camera(camera)
    {}

    ~RayMarchWorker();

    void run();
};

#endif // RAYMARCHWORKER_H
