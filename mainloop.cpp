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


void MainLoop::render()
{
    rayMarcher.render();
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
    rayMarcher(camera, window, 8),
    window(800, 600),
    camera(window.getGeometry())
{
    init();
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
        std::cout << "kek\n";
        customTime.calcDeltaTime();
        prepareScreen();
        processEvents();
        update();
        render();
        window.rendererPresent();
        delayFps();
    }
}
