#include <sp/Window.h>
#include <SDL2/SDL.h>

std::shared_ptr<sp::Window> sp::Window::Open(unsigned int w, unsigned int h, std::string title) {
#if defined(_OPENGL)
    return sp::OGLWindow::Open(w, h, std::move(title));
#endif
}

std::shared_ptr<sp::OGLWindow> sp::OGLWindow::Open(unsigned int w, unsigned h, std::string title) {
    auto window = std::make_shared<OGLWindow>();
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    window->create(w, h, title);

    return window;
}

void sp::OGLWindow::create(unsigned int w, unsigned h, const std::string& title) {
    win_handle = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,

                                  w, h, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    glContext = SDL_GL_CreateContext(win_handle);
    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
    SDL_GL_SetSwapInterval(1);
}

sp::OGLWindow::OGLWindow() : Window() {}

sp::OGLWindow::~OGLWindow() {
    if(win_handle != nullptr) {
        SDL_DestroyWindow(win_handle);
        win_handle = nullptr;
    }
}

void sp::OGLWindow::swapBuffer() {
    SDL_GL_SwapWindow(win_handle);
}
