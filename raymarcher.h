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
    std::vector<glm::vec3> pixels;
    Window &window;
    std::vector<RayMarchWorker> workers;
    std::mutex getJobMutex;
    std::mutex setPixelMutex;
    bool isNewJobsAvailable = false;
    bool isStoppingWorkers = false;
    glm::ivec2 currentPixelPosition;

public:
    RayMarcher(Camera &camera, Window &window, uint workerNumber = 1);

    void runWorkers();

    void stopWorkers();

    const glm::ivec2 getJob();

    void render();
private:
    void setColors();
};

#endif // RAYMARCHER_H
