#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <cstdio>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
bool quit = false;

SDL_Event e;

// Start up SDL and create a window
bool init();

void close();

void printShaderLog(GLuint shader);

void printProgramLog(GLuint program);

bool initGL();

void render();

SDL_Window *gWindow = nullptr;

SDL_GLContext gContext;

//Graphics program
GLuint gProgramID = 0;
GLuint gVertexPos2DLocation = 0;
GLuint gVBO = 0;
GLuint gVAO = 0;

bool gRenderQuad = true;

int main(int argc, char *args[]) {
    if (!init()) {
        printf("Failed to initialize!\n");
    } else {
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
            render();
            SDL_GL_SwapWindow(gWindow);
        }
    }
    close();
    return 0;
}

bool init() {
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    } else {
        //Use OpenGL 3.3 core
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        //Create window
        gWindow = SDL_CreateWindow("SDL GL Triangle", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                   SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
        if (gWindow == nullptr) {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        } else {
            // Create OpenGL context
            gContext = SDL_GL_CreateContext(gWindow);
            if (gContext == nullptr) {
                printf("OpenGL context could not be created! SDL Error0: %s\n", SDL_GetError());
                success = false;
            } else {
                // Setup GLAD
                if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
                    printf("Failed to initialize GLAD!");
                    success = false;
                }

                // Use Vsync
                if (SDL_GL_SetSwapInterval(1) < 0) {
                    printf("Warning: Unable to set Vsync! SDL Error: %s\n", SDL_GetError());
                }

                // Initialize OpenGL
                if (!initGL()) {
                    printf("Unable to initialize OpenGL!\n");
                    success = false;
                }
            }
        }
    }
    return success;
}

bool initGL() {
    bool success = true;

    gProgramID = glCreateProgram();
    //Create vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    //Get vertex source
    const GLchar *vertexShaderSource[] =
            {
                    "#version 330 core\nin vec3 LVertexPos2D; void main() { gl_Position = vec4( LVertexPos2D.x, LVertexPos2D.y, 0, 1 ); }"
            };

    //Set vertex source
    glShaderSource(vertexShader, 1, vertexShaderSource, nullptr);

    //Compile vertex source
    glCompileShader(vertexShader);

    //Check vertex shader for errors
    GLint vShaderCompiled = GL_FALSE;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vShaderCompiled);
    if (vShaderCompiled != GL_TRUE) {
        printf("Unable to compile vertex shader %d!\n", vertexShader);
        printShaderLog(vertexShader);
        success = false;
    } else {
        //Attach vertex shader to program
        glAttachShader(gProgramID, vertexShader);


        //Create fragment shader
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        //Get fragment source
        const GLchar *fragmentShaderSource[] =
                {
                        "#version 330 core\nout vec4 LFragment; void main() { LFragment = vec4( 0.0, 1.0, 1.0, 1.0 ); }"
                };

        //Set fragment source
        glShaderSource(fragmentShader, 1, fragmentShaderSource, nullptr);

        //Compile fragment source
        glCompileShader(fragmentShader);

        //Check fragment shader for errors
        GLint fShaderCompiled = GL_FALSE;
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fShaderCompiled);
        if (fShaderCompiled != GL_TRUE) {
            printf("Unable to compile fragment shader %d!\n", fragmentShader);
            printShaderLog(fragmentShader);
            success = false;
        } else {
            //Attach fragment shader to program
            glAttachShader(gProgramID, fragmentShader);


            //Link program
            glLinkProgram(gProgramID);

            //Check for errors
            GLint programSuccess = GL_TRUE;
            glGetProgramiv(gProgramID, GL_LINK_STATUS, &programSuccess);
            if (programSuccess != GL_TRUE) {
                printf("Error linking program %d!\n", gProgramID);
                printProgramLog(gProgramID);
                success = false;
            } else {
                //Get vertex attribute location
                gVertexPos2DLocation = glGetAttribLocation(gProgramID, "LVertexPos2D");
                if (gVertexPos2DLocation == -1) {
                    printf("LVertexPos2D is not a valid glsl program variable!\n");
                    success = false;
                } else {
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
                }
            }
        }
    }
    return success;
}

void printProgramLog(GLuint program) {
    //Make sure name is shader
    if (glIsProgram(program)) {
        //Program log length
        int infoLogLength = 0;
        int maxLength = infoLogLength;

        //Get info string length
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        //Allocate string
        auto infoLog = new char[maxLength];

        //Get info log
        glGetProgramInfoLog(program, maxLength, &infoLogLength, infoLog);
        if (infoLogLength > 0) {
            //Print Log
            printf("%s\n", infoLog);
        }

        //Deallocate string
        delete[] infoLog;
    } else {
        printf("Name %d is not a program\n", program);
    }
}

void printShaderLog(GLuint shader) {
    //Make sure name is shader
    if (glIsShader(shader)) {
        //Shader log length
        int infoLogLength = 0;
        int maxLength = infoLogLength;

        //Get info string length
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        //Allocate string
        auto infoLog = new char[maxLength];

        //Get info log
        glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog);
        if (infoLogLength > 0) {
            //Print Log
            printf("%s\n", infoLog);
        }

        //Deallocate string
        delete[] infoLog;
    } else {
        printf("Name %d is not a shader\n", shader);
    }
}

void render() {
    //Clear color buffer
    glClear(GL_COLOR_BUFFER_BIT);

    //Render quad
    if (gRenderQuad) {
        //Bind program
        glUseProgram(gProgramID);

        glBindVertexArray(gVAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(NULL);
    }
}

void close() {
    glDeleteProgram(gProgramID);

    SDL_DestroyWindow(gWindow);
    gWindow = nullptr;

    SDL_Quit();
}