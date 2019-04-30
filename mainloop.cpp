#include "mainloop.h"



void MainLoop::init()
{

}

void MainLoop::prepareScreen()
{
    window.clear();
}

void MainLoop::processEvents()
{
    SDL_Event event;
    while ( SDL_PollEvent(&event) )
    {
        switch(event.type)
        {
        case SDL_QUIT:
            isRunning = false;
            break;
        case SDL_MOUSEBUTTONDOWN:
            break;
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                isRunning = false;
                break;
            }
            break;
        }
    }
}

void MainLoop::update()
{

}

//char colorToChar(const glm::vec3 &color)
//{
//    const std::vector<char> charSet = {' ', '.', ',', ':', ';', 'i', 't', '*', '@'};
//    const uint charSetSize = charSet.size();
//    float intensity = color.x;
//    for(int i = 0; i < charSetSize; i++)
//    {
//        if(intensity < 1.0f/charSetSize*i)
//            return charSet[i];
//    }
//    return charSet.back();
//}

void MainLoop::render()
{
    rayMarcher.render();
    auto pixels = rayMarcher.getPixels();
    const uint pixelCount = pixels->size();
    const glm::ivec2 geometry = window.getGeometry();
    for (uint i = 0; i < pixelCount; i++)
    {
        const uint pixelX = i%geometry.x;
        const uint pixelY = i/geometry.x;
        const glm::vec3 color = pixels->operator[](i);
        window.setPixel(glm::ivec2(pixelX, pixelY), color);
    }
//    uint prevY = 0;
//    for (uint i = 0; i < pixelCount; i++)
//    {
//        const uint pixelX = i%geometry.x;
//        const uint pixelY = i/geometry.x;
//        if(prevY != pixelY)
//        {
//            std::cout << "\n";
//            prevY = pixelY;
//        }
//        const glm::vec3 color = pixels->operator[](i);
//        std::cout << colorToChar(color);
//    }
//    std::cout << "\n";
}


void MainLoop::delayFps()
{
    if(1000.0f/fpsCap > customTime.deltaTime*1000.0f)
    {
        Uint32 delay = Uint32(1000.0f/fpsCap-customTime.deltaTime*1000.0f);
        SDL_Delay(delay);
    }
}

MainLoop::MainLoop():
    window(800, 600),
    camera(window.getGeometry()),
    rayMarcher(window.getGeometry(), 8)
{
    init();
    rayMarcher.setCameraMatrices(camera.getView(), camera.getProjection(), camera.getPosition());
}

MainLoop::~MainLoop()
{
    rayMarcher.stopWorkers();
}

void MainLoop::run()
{
    rayMarcher.runWorkers();
    isRunning = true;

    while(isRunning)
    {
        std::cout << "Fps: " << 1.0f/customTime.deltaTime << std::endl;
        customTime.calcDeltaTime();
        prepareScreen();
        processEvents();
        update();
        render();
        window.rendererPresent();
        delayFps();
    }
}
