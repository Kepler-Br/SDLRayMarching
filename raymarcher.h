#ifndef RAYMARCHER_H
#define RAYMARCHER_H

#include <glm/glm.hpp>
#include <vector>
#include <mutex>
#include <functional>

#include "raymarchworker.h"
#include "camera.h"
#include "window.h"
#include "array"

class RayMarcher
{
    Window &window;
    std::vector<RayMarchWorker> workers;
    std::mutex getJobMutex;
    std::mutex setPixelMutex;
    bool isNewJobsAvailable = false;
    bool isStoppingWorkers = false;
    glm::ivec2 currentPixelPosition;

public:
    RayMarcher(Camera &camera, Window &window, uint workerNumber = 1):
        window(window)
    {
        workers.reserve(workerNumber);
        std::function<const glm::ivec2()> jobFunction = std::bind(&RayMarcher::getJob, this);

        for(uint i = 0; i < workerNumber; i++)
        {
            RayMarchWorker worker(jobFunction, getJobMutex, setPixelMutex, window, camera);
            workers.push_back(worker);
        }
    }

    void runWorkers()
    {
        for(auto &worker: workers)
            worker.run();
    }

    void stopWorkers()
    {
        isStoppingWorkers = true;
    }

    const glm::ivec2 getJob()
    {
        if(isStoppingWorkers == true)
            return glm::vec2(-2);

        const glm::ivec2 &windowGeometry = window.getGeometry();

        currentPixelPosition.x++;
        if(currentPixelPosition.x == windowGeometry.x)
        {
            currentPixelPosition.x = 0;
            currentPixelPosition.y++;
        }

        if(currentPixelPosition.y == windowGeometry.y)
        {
            isNewJobsAvailable = false;
            currentPixelPosition = glm::ivec2(0, 0);
        }

        if(isNewJobsAvailable == false)
            return glm::vec2(-1);
        return currentPixelPosition;
    }

    void render()
    {
        currentPixelPosition = glm::ivec2(0, 0);
        isNewJobsAvailable = true;
        while(isNewJobsAvailable)
            std::this_thread::sleep_for(std::chrono::microseconds(10));
    }
};

#endif // RAYMARCHER_H
