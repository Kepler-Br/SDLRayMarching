#include "mainloop.h"



void MainLoop::init()
{
    if ( SDL_Init(SDL_INIT_EVERYTHING) < 0 )
    {
        std::cout << "Unable to init SDL, error: " << SDL_GetError() << std::endl;
        exit(1);
    }
    SDL_CreateWindowAndRenderer(windowGeometry.x, windowGeometry.y, SDL_WINDOW_SHOWN, &window, &renderer);
    if(window == nullptr || renderer == nullptr)
    {
        std::cout << "Unable to create window: " << SDL_GetError() << std::endl;
        exit(1);
    }
    SDL_SetWindowTitle(window, "Software raymarching demo.");
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}

void MainLoop::prepareScreen()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
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

float getDistance(glm::vec3 currentMarchingLocation)
{
    glm::vec3 sphere = glm::vec3(0.0f, 1.0f, 6.0f);
    float sphereRadius = 1.0f;
    float sphereDist = glm::length(currentMarchingLocation-sphere)-sphereRadius;

    float planeDistance = currentMarchingLocation.y;

    float distance = glm::min(sphereDist, planeDistance);
    return distance;

}

float MainLoop::rayMarch(glm::vec3 rayDirection)
{
    glm::vec3 rayOrigin = camera.getPosition();
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

void MainLoop::render()
{
    for(int x = 0; x < windowGeometry.x; x++)
    {
        for(int y = 0; y < windowGeometry.y; y++)
        {
            glm::vec3 world = camera.screenToWorld(glm::ivec2(x, y));
            glm::ivec2 position(x, y);
            float distance = rayMarch(world)/20.0f;
            distance = distance > 1.0f?1.0f:distance;
            setPixel(glm::vec3(distance), position);
        }
    }

}

void MainLoop::setPixel(glm::vec3 color, glm::ivec2 position)
{
    SDL_SetRenderDrawColor(renderer, static_cast<Uint8>(color.r*255),
                           static_cast<Uint8>(color.g*255),
                           static_cast<Uint8>(color.b*255),
                           255);
    SDL_RenderDrawPoint(renderer, position.x, position.y);
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
    camera(windowGeometry)
{
    init();
}

MainLoop::~MainLoop()
{
    if(renderer != nullptr)
        SDL_DestroyRenderer(renderer);
    if(window != nullptr)
        SDL_DestroyWindow(window);
    SDL_Quit();
}

void MainLoop::run()
{
    isRunning = true;

    while(isRunning)
    {
        std::cout << "kek\n";
        customTime.calcDeltaTime();
        prepareScreen();
        processEvents();
        update();
        render();
        SDL_RenderPresent(renderer);
        delayFps();
    }
}
