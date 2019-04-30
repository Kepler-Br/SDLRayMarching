#ifndef RAYMARCHER_H
#define RAYMARCHER_H

#include <glm/glm.hpp>
#include <vector>
#include <mutex>
#include <functional>

#include "raymarchworker.h"
#include "raymarchjob.h"

class RayMarcher
{
    std::vector<glm::vec3> pixels;

    glm::ivec2 renderViewportSize;
    std::vector<RayMarchWorker> workers;
    std::mutex getJobMutex;
    bool isNewJobsAvailable = false;
    bool isStoppingWorkers = false;

    RayMarchJob currentJob;

    bool isAllJobsDone();
public:
    RayMarcher(const glm::ivec2 renderViewportSize, const uint workerNumber = 1);

    void setCameraMatrices(const glm::mat4 &view, const glm::mat4 &projection, const glm::vec3 cameraPosition);
    void runWorkers();
    void stopWorkers();

    const RayMarchJob getJob();

    const std::vector<glm::vec3> *getPixels();

    void render();
};

#endif // RAYMARCHER_H
