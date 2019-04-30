#include "raymarcher.h"

RayMarcher::RayMarcher(Camera &camera, Window &window, uint workerNumber):
    window(window)
{
    pixels.resize(window.getGeometry().x*window.getGeometry().y, glm::vec3(0.0f));
    workers.reserve(workerNumber);
    std::function<const glm::ivec2()> jobFunction = std::bind(&RayMarcher::getJob, this);

    for(uint i = 0; i < workerNumber; i++)
    {
        RayMarchWorker worker(jobFunction, getJobMutex, setPixelMutex, pixels, camera, workerNumber);
        workers.push_back(worker);
    }
}

void RayMarcher::runWorkers()
{
    for(auto &worker: workers)
        worker.run();
}

void RayMarcher::stopWorkers()
{
    isStoppingWorkers = true;
}

const glm::ivec2 RayMarcher::getJob()
{

    if(isStoppingWorkers == true)
        return glm::ivec2(-2);
    if(isNewJobsAvailable == false)
        return glm::ivec2(-1);
    const glm::ivec2 &windowGeometry = window.getGeometry();

    const int frameSize = windowGeometry.x*windowGeometry.y/(int)workers.size();

    currentPixelPosition.x += frameSize;
    currentPixelPosition.y = frameSize;
    if(currentPixelPosition.x >= windowGeometry.x*windowGeometry.y)
    {
        isNewJobsAvailable = false;
        currentPixelPosition = glm::ivec2(0, 0);
    }

    return currentPixelPosition;
}

void RayMarcher::setColors()
{
    const glm::ivec2 &geometry = window.getGeometry();
    for(uint currentPixel = 0; currentPixel < pixels.size(); currentPixel++)
    {
        const glm::ivec2 pixelPosition(currentPixel%geometry.x, currentPixel/geometry.x);
        const glm::vec3 &color = pixels[currentPixel];
        window.setPixel(pixelPosition, color);
    }
}

void RayMarcher::render()
{
    currentPixelPosition = glm::ivec2(0, 0);
    isNewJobsAvailable = true;
    bool workDone = false;
    while(true)
    {
        std::this_thread::sleep_for(std::chrono::microseconds(10));
        if(!isNewJobsAvailable)
        {
            for(auto &worker : workers)
            {
                workDone = true;
                if(!worker.isWorkDone())
                {
                    workDone = false;
                    break;
                }
            }
            if(workDone)
                break;
        }
    }
    setColors();
}
