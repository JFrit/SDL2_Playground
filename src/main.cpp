#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <sp/ShaderProgram.h>

#include "sp/Window.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 960;
bool quit = false;

SDL_Event e;

void Close();

void PrintShaderLog(GLuint shader);

void PrintProgramLog(GLuint program);

bool InitGL();

void Render();

//Graphics program
GLuint gProgramID = 0;
GLuint gVertexPos2DLocation = 0;
GLuint gVBO = 0;
GLuint gVAO = 0;

bool gRenderQuad = true;

int main(int argc, char *args[]) {
    std::shared_ptr<sp::Window> window = sp::Window::Open(SCREEN_WIDTH, SCREEN_HEIGHT, "TestGL");
    InitGL();
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        quit = true;
                        break;
                }
            }
        }
        Render();
        window->SwapBuffer();
    }
    Close();
    return 0;
}

bool InitGL() {
    auto shader = sp::ShaderProgram::UsingFiles("shaders/basic.vert", "shaders/basic.frag");
    shader->Use();

    //Initialize clear color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    //VBO data
    GLfloat vertexData[] =
            {
                    -0.5f, -0.5f, 0.0f,
                    0.5f, -0.5f, 0.0f,
                    0.0f,  0.5f, 0.0f
            };

    //Create VAO
    glGenVertexArrays(1, &gVAO);
    glBindVertexArray(gVAO);

    //Create VBO
    glGenBuffers(1, &gVBO);
    glBindBuffer(GL_ARRAY_BUFFER, gVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    glVertexAttribPointer(gVertexPos2DLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
                          nullptr);


    glEnableVertexAttribArray(gVertexPos2DLocation);
    return true;
}

void Render() {
    //Clear color buffer
    glClear(GL_COLOR_BUFFER_BIT);

    //Render quad
    if (gRenderQuad) {
        glBindVertexArray(gVAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
}

void Close() {
    SDL_Quit();
}