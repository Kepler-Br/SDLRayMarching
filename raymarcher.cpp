#include "raymarcher.h"

RayMarcher::RayMarcher(const glm::ivec2 renderViewportSize, const uint workerNumber):
    renderViewportSize(renderViewportSize)
{
    currentJob.pixelsToRender = static_cast<uint>(renderViewportSize.x*renderViewportSize.y)/workerNumber;
    currentJob.newJobAvailable = true;
    currentJob.isShuttingDown = false;
    pixels.resize(static_cast<uint>(renderViewportSize.x*renderViewportSize.y), glm::vec3(0.0f));
    std::function<const RayMarchJob()> jobFunction = std::bind(&RayMarcher::getJob, this);
    workers.reserve(workerNumber);
    for(uint i = 0; i < workerNumber; i++)
    {
        RayMarchWorker worker(jobFunction, getJobMutex, pixels, renderViewportSize);
        workers.push_back(worker);
    }
}

void RayMarcher::setCameraMatrices(const glm::mat4 &view, const glm::mat4 &projection, const glm::vec3 cameraPosition)
{
    for(auto &worker: workers)
        worker.setCameraMatrices(view, projection, cameraPosition);
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

const RayMarchJob RayMarcher::getJob()
{
    RayMarchJob newJob = currentJob;
    if(isStoppingWorkers == true)
    {
        newJob.isShuttingDown = true;
        return newJob;
    }
    if(isNewJobsAvailable == false)
    {
        newJob.newJobAvailable = false;
        return newJob;
    }

//    const int frameSize = renderViewportSize.x*renderViewportSize.y/static_cast<int>(workers.size());

//    currentPixelPosition.y = jobSliceSize;


//    newJob =
    currentJob.pixelNumber += currentJob.pixelsToRender;
    if(currentJob.pixelNumber + currentJob.pixelsToRender > pixels.size())
    {
        isNewJobsAvailable = false;
//        newJob.newJobAvailable = false;
//        return newJob;
}
    return newJob;
}

const std::vector<glm::vec3> *RayMarcher::getPixels()
{
    return &pixels;
}

//void RayMarcher::setColors()
//{
//    const glm::ivec2 &geometry = window.getGeometry();
//    for(uint currentPixel = 0; currentPixel < pixels.size(); currentPixel++)
//    {
//        const glm::ivec2 pixelPosition(currentPixel%static_cast<uint>(geometry.x), currentPixel/static_cast<uint>(geometry.x));
//        const glm::vec3 &color = pixels[currentPixel];
//        window.setPixel(pixelPosition, color);
//    }
//}

bool RayMarcher::isAllJobsDone()
{
    for(auto &worker : workers)
        if(!worker.isWorkDone())
            return false;
    return true;
}

void RayMarcher::render()
{
    using namespace std::this_thread;
    using namespace std::chrono;

    currentJob.pixelNumber = 0;
    isNewJobsAvailable = true;
    while(true)
    {
        if(!isNewJobsAvailable)
            if(!isNewJobsAvailable && isAllJobsDone())
                break;
        sleep_for(microseconds(10));
    }
}
