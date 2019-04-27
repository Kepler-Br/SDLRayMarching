#ifndef MAINLOOP_H
#define MAINLOOP_H

#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <iostream>

#include "customtime.h"
#include "camera.h"

class MainLoop
{
private:
    const glm::ivec2 windowGeometry = {800, 600};
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;

    bool isRunning = false;
    const float fpsCap = 1.0f;

    CustomTime customTime;
    Camera camera;

    void init();
    void prepareScreen();
    void processEvents();
    void update();
    void render();
    void delayFps();
    void setPixel(glm::vec3 color, glm::ivec2 position);

    float rayMarch(glm::vec3 rayOrigin, glm::vec3 rayDirection, float minDistanceToSurface = 0.1f);
    float getDistance(glm::vec3 currentMarchingLocation);
    float getLight(glm::vec3 surfacePosition);
    glm::vec3 getNormal(glm::vec3 surfacePosition);
public:
    MainLoop();
    ~MainLoop();
    void run();

};

#endif // MAINLOOP_H
