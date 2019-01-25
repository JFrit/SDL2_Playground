#ifndef SDL2_PLAYGROUND_WINDOW_H
#define SDL2_PLAYGROUND_WINDOW_H

#if defined(_OPENGL)
#include "glad/glad.h"
#endif

#include <memory>
#include <SDL2/SDL_video.h>

namespace sp {

    // A window used for rendering. There's no reason to copy
    // an entire window so copying has been disabled.
    class Window {
    public:
        static std::shared_ptr<Window> Open(unsigned int w, unsigned h, std::string title);

        Window() = default;
        Window(Window& other) = delete;
        Window& operator=(Window& other) = delete;
        virtual void swapBuffer() = 0;

        virtual ~Window() = default;
    };

    class OGLWindow : public Window {
    public:
        static std::shared_ptr<OGLWindow> Open(unsigned int w, unsigned h, std::string title);

        OGLWindow();
        OGLWindow(OGLWindow& other) = delete;
        OGLWindow& operator=(OGLWindow& other) = delete;
        void swapBuffer() override;

        ~OGLWindow() override;

    private:
        SDL_Window* win_handle;
        SDL_GLContext glContext;
        virtual void create(unsigned int w, unsigned h, const std::string& title);
    };
}

#endif //SDL2_PLAYGROUND_WINDOW_H
