#ifndef MAINLOOP_H
#define MAINLOOP_H

#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <iostream>

#include "customtime.h"
#include "raymarcher.h"
#include "window.h"
#include "camera.h"

class MainLoop
{
private:
    bool isRunning = false;
    const float fpsCap = 60.0f;

    CustomTime customTime;
    Window window;
    RayMarcher rayMarcher;
    Camera camera;

    void init();
    void prepareScreen();
    void processEvents();
    void update();
    void render();
    void delayFps();
public:
    MainLoop();
    ~MainLoop();
    void run();

};

#endif // MAINLOOP_H
