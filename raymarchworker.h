#ifndef RAYMARCHWORKER_H
#define RAYMARCHWORKER_H
#define GLM_ENABLE_EXPERIMENTAL
#include <functional>
#include <glm/glm.hpp>
#include <mutex>
#include <thread>
#include <vector>

#include "raymarchjob.h"

class RayMarchWorker
{
    std::function<RayMarchJob()> getJob;
    std::mutex &getJobMutex;
    glm::ivec2 renderViewportSize;
    std::vector<glm::vec3> *pixels;
    bool workDone = true;

    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 VP;
    glm::mat4 inverseVP;
    glm::vec3 cameraPosition;

    std::thread *thread = nullptr;

    bool isRunning = false;

    static float getDistance(glm::vec3 currentMarchingLocation);
    static float rayMarch(glm::vec3 &rayOrigin, glm::vec3 &rayDirection);
    static glm::vec3 screenToWorld(const glm::ivec2 &screenPosition,
                                   const glm::ivec2 &renderViewportSize,
                                   const glm::mat4 &inverseVP);

    void threadFunction();

public:
    RayMarchWorker(std::function<RayMarchJob()> jobFunction,
                   std::mutex &getJobMutex,
                   std::vector<glm::vec3> &pixels,
                   const glm::ivec2 &renderViewportSize):
        getJob(jobFunction),
        getJobMutex(getJobMutex),
        renderViewportSize(renderViewportSize),
        pixels(&pixels)
    {}

    ~RayMarchWorker();

    void run();
    bool isWorkDone() const;
    void setCameraMatrices(const glm::mat4 &view, const glm::mat4 &projection, const glm::vec3 &position);
    void updateMatrices();
};

#endif // RAYMARCHWORKER_H
