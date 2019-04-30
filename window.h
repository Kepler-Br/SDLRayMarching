#ifndef WINDOW_H
#define WINDOW_H

#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <string>

class Window
{
private:
    glm::ivec2 geometry;
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;

    /**
     *  \brief Initializes SDL2 renderer and window.
     *
     *  \param geometry geometry of the window.
     */
    void init(const glm::ivec2 &geometry);

public:
    Window(const glm::ivec2 &geometry, const std::string &windowTitle = "SDL2 window");
    Window(const int x, const int y, const std::string &windowTitle = "SDL2 window");
    ~Window();

    /**
     *  \brief Sets window title.
     *
     *  \param title String to which set window title.
     */
    void setWindowTitle(const std::string &title);

    /**
     *  \brief Sets window position on screen.
     *
     *  \param position Integer vector to where move the window.
     */
    void setWindowPosition(const glm::ivec2 &position);

    /**
     *  \brief Returns geometry of the SDL2 window.
     *
     *  \return width and height of the window in int.
     */
    const glm::ivec2 getGeometry() const;

    /**
     *  \brief Sets pixel at position to color.
     *
     *  \param position coordinates of the point.
     *  \param color value from 0.0f to 1.0f.
     */
    void setPixel(const glm::ivec2 &position, const glm::vec3 &color);

    /**
     *  \brief Sets pixel at position to color.
     *
     *  \param position coordinates of the point.
     *  \param color value from 0 to 255.
     */
    void setPixel(const glm::ivec2 &position, const glm::ivec3 &color);
    /**
     *  \brief Clears the renderer.
     *
     *  \param color Color value from 0.0f to 1.0f.
     */
    void clear(const glm::vec3 &color);

    /**
     *  \brief Clears the renderer.
     *
     *  \param color Color value from 0 to 255.
     */
    void clear(const glm::ivec3 &color);

    /**
     *  \brief Clears the renderer with black color.
     */
    void clear();

    /**
     *  \brief Updates renderer with new pixels.
     */
    void rendererPresent()
    {
        SDL_RenderPresent(renderer);
    }
};

#endif // WINDOW_H
