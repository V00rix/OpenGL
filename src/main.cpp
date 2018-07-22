// OpenGL
#include <GL/glew.h>
// other stuff
#include <iostream>
#include <fstream>
#include <cmath>
#include <sstream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include "elements/Cube.h"
#include "util/util.h"
#include "elements/Square.h"
#include "glfw3.h"
#include "light/light.h"

#define _DEBUG 1

//region Helpers
/**
 * Main window
 */
static GLFWwindow *window;

/**
 * Initialize GLFW
 * @return
 */
static const bool initGLFW() {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return false;
    }

    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    return true;
}

/**
 * Open a window and create its OpenGL context
 * @return
 */
static bool createWindow() {
    window = glfwCreateWindow(1024, 768, "OpenGL", nullptr, nullptr);
    if (window == nullptr) {
        fprintf(stderr, "Failed to open GLFW window.\n");
        getchar();
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    return true;
}

/**
 * Initialize GLEW
 * @return
 */
static bool initGLEW() {
    glewExperimental = GL_TRUE; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return false;
    }
    return true;
}

/**
 * Compile and load shaders
 * @param vs Path to vertex shader file
 * @param fs Path to fragment shader file
 * @return Program id
 */
static unsigned int loadShaders(const char *vs, const char *fs) {

    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Read the Vertex Shader code from the file
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vs, std::ios::in);
    if (VertexShaderStream.is_open()) {
        std::stringstream sstr;
        sstr << VertexShaderStream.rdbuf();
        VertexShaderCode = sstr.str();
        VertexShaderStream.close();
    } else {
        printf("Couldn't open shader file: %s.\n", vs);
        getchar();
        return 0;
    }

    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fs, std::ios::in);
    if (FragmentShaderStream.is_open()) {
        std::stringstream sstr;
        sstr << FragmentShaderStream.rdbuf();
        FragmentShaderCode = sstr.str();
        FragmentShaderStream.close();
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Compile Vertex Shader
    printf("Compiling shader : %s\n", vs);
    char const *VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer, nullptr);
    glCompileShader(VertexShaderID);

    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> VertexShaderErrorMessage((unsigned) InfoLogLength + 1);
        glGetShaderInfoLog(VertexShaderID, InfoLogLength, nullptr, &VertexShaderErrorMessage[0]);
        printf("%s\n", &VertexShaderErrorMessage[0]);
    }

    // Compile Fragment Shader
    printf("Compiling shader : %s\n", fs);
    char const *FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, nullptr);
    glCompileShader(FragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> FragmentShaderErrorMessage((unsigned) InfoLogLength + 1);
        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, nullptr, &FragmentShaderErrorMessage[0]);
        printf("%s\n", &FragmentShaderErrorMessage[0]);
    }

    // Link the program
    printf("Linking program\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> ProgramErrorMessage((unsigned) InfoLogLength + 1);
        glGetProgramInfoLog(ProgramID, InfoLogLength, nullptr, &ProgramErrorMessage[0]);
        printf("%s\n", &ProgramErrorMessage[0]);
    }

    glDetachShader(ProgramID, VertexShaderID);
    glDetachShader(ProgramID, FragmentShaderID);

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    return ProgramID;
}

/**
 * Error handler function
 * @param source
 * @param type
 * @param id
 * @param severity
 * @param length
 * @param message
 * @param userParam
 */
static void APIENTRY
openglCallbackFunction(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message,
                       const void *userParam) {
    (void) source;
    (void) type;
    (void) id;
    (void) severity;
    (void) length;
    (void) userParam;
    fprintf(stderr, "%s\n", message);
    if (severity == GL_DEBUG_SEVERITY_HIGH) {
        fprintf(stderr, "Aborting...\n");
        abort();
    }
}

static elements::Square generateText(const char *string, GLuint texture) {
    const glm::vec2 uv[4] = {
            glm::vec2(.0f, .0f),
            glm::vec2(1.0f/16.f, .0f),
            glm::vec2(1.0f/16.f, 1.0f/16.f),
            glm::vec2(.0f, 1.0f/16.f),
    };

    return elements::Square(glm::vec3(-.5f, -.5f, .0f), .2f, uv);
}
//endregion

int main() {

    //region Initialization
    if (!initGLFW()) { return -1; }
    if (!createWindow()) { return -1; }
    if (!initGLEW()) { return -1; }

#if _DEBUG
    // Request a debug context.
    std::cout << "Debug output enabled!" << std::endl;
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(openglCallbackFunction, nullptr);
#endif

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    //endregion

    // shader
    GLuint shader = loadShaders("resources/shaders/vertex.glsl",
                                "resources/shaders/fragment.glsl"); // this is relative to target binary u_color
    glUseProgram(shader);

    // uniforms
    int u_model = glGetUniformLocation(shader, "model");
    int u_view = glGetUniformLocation(shader, "view");
    int u_projection = glGetUniformLocation(shader, "projection");

//    int u_light_position = glGetUniformLocation(shader, "light_position");

    int u_texture_sampler = glGetUniformLocation(shader, "texture_sampler");
    auto u_directional_light = light::uni::getDirectional(shader, "directional_light");
    light::uni::u_point u_point_light[2] = {
            light::uni::getPoint(shader, "point_lights[0]"),
            light::uni::getPoint(shader, "point_lights[1]")
    };
    auto u_point_lights_count = glGetUniformLocation(shader, "point_lights_count");

    int u_camera_position = glGetUniformLocation(shader, "camera_position");
    int u_specular_intensity = glGetUniformLocation(shader, "specular_intensity");
    int u_specular_power = glGetUniformLocation(shader, "specular_power");

    int u_draw_ui = glGetUniformLocation(shader, "draw_ui");

    // Clear color
    glClearColor(0.f, 0.f, 0.f, 0.f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Cull triangles which normal is not towards the camera
    glEnable(GL_CULL_FACE);

    // region load mesh
    std::vector<util::Vertex> vertices;
    util::loadOBJ("resources/meshes/spaceship.obj", vertices);
    unsigned vertexCount = vertices.size();

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(util::Vertex) * vertexCount, &vertices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, util::Vertex::size, nullptr);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, util::Vertex::size, (void *) util::Vertex::color_offset);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, util::Vertex::size, (void *) util::Vertex::uv_offset);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //endregion

    // load textures
    GLuint ascensionTexture = util::loadDDS("resources/textures/ascensionLogo.dds");
    if (!ascensionTexture) {
        fprintf(stdout, "Couldn't load texture.");
        return -1;
    }
    GLuint ascensionTexture_bmp = util::loadBMP("resources/textures/ascensionLogo.bmp");
    if (!ascensionTexture_bmp) {
        fprintf(stdout, "Couldn't load texture.");
        return -1;
    }
    GLuint fontTexture = util::loadBMP("resources/fonts/font.bmp");
    if (!fontTexture) {
        fprintf(stdout, "Couldn't load texture.");
        return -1;
    }

    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ascensionTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, ascensionTexture_bmp);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, fontTexture);

    glUniform1i(u_texture_sampler, 0);

    {
        elements::Cube myCube({.0f, .0f, .0f}, .5f);
        elements::Cube myCube2({.0f, .0f, -1.0f}, .75f);
        elements::Square mySquare({-2.5f, -5.f, 0.f}, 5.f);

        // animation
        float r = .5f, increment = 0.005f;

        float yoff, xoff = yoff = 0.0f;
        float yrot, xrot = yrot = 0.0f;

        double lastTime = glfwGetTime();
        int frames = 0;

        auto myText = generateText("Hello, world!", fontTexture);

        do {
            double currentTime = glfwGetTime();
            frames++;

            if (currentTime - lastTime >= 1.0) {
                printf("%fms\n", 1000.f / frames);
                frames = 0;
                lastTime = currentTime;
            }


            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            //region Get input
            // Get mouse position
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);

            // Move forward
            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
                yoff += 0.1f;
            }
            // Move backward
            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
                yoff -= 0.1f;
            }
            // Strafe right
            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
                xoff -= 0.1f;
            }
            // Strafe left
            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
                xoff += 0.1f;
            }

            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
                yrot += 0.1f;
            }
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
                yrot -= 0.1f;
            }
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
                xrot += 0.1f;
            }
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
                xrot -= 0.1f;
            }
            //endregion

            glUniform1i(u_draw_ui, false);


            glm::vec3 cameraPosition(4, 3, 3); // Camera is at (4,3,3), in World Space

            // MVP
            // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
            glm::mat4 projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

            // Camera matrix
            glm::mat4 view = glm::lookAt(
                    cameraPosition,
                    glm::vec3(0, 0, 0), // and looks at the origin
                    glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
            );

            // model matrix : an identity matrix (model will be at the origin)
            glm::mat4 model = glm::mat4(1.0f);

            glm::mat4 translated = glm::translate(projection, glm::vec3(.0f, .0f, 0.f));
            glm::mat4 rotated = glm::rotate(glm::rotate(model, xrot, {1.0f, .0f, .0f}),
                                            yrot, {0.0f, 1.0f, 0.f});

            glUniformMatrix4fv(u_model, 1, GL_FALSE, &rotated[0][0]);
            glUniformMatrix4fv(u_view, 1, GL_FALSE, &view[0][0]);
            glUniformMatrix4fv(u_projection, 1, GL_FALSE, &translated[0][0]);

            glUniform3f(u_camera_position, cameraPosition.x, cameraPosition.y, cameraPosition.z);
            glUniform1f(u_specular_intensity, 5.0f);
            glUniform1f(u_specular_power, 32);

            //region Main loop
            float ambientIntensity = .05f;
            glm::vec3 direction(-1.f, -.5f, .3f);

            light::Directional sun({glm::vec3(1.f, .0f, .0f), ambientIntensity, .2f}, direction);
            light::Point p1({glm::vec3(1.f), .01f, 1.f}, glm::vec3(1.5f - xoff, yoff + -.5f, .0f), {1.f, .01f, .015f});
            light::Point p2({glm::vec3(.3f, 1.f, .4f), .1f, 2.f}, glm::vec3(-1.f, 3.f, .0f), {1.f, .01f, .015f});

            light::uni::setDirectional(u_directional_light, sun);
            glUniform1i(u_point_lights_count, 2);

            light::uni::setPoint(u_point_light[1], p2);
            light::uni::setPoint(u_point_light[0], p1);

            glUniform1i(u_texture_sampler, 0);

            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glDrawArrays(GL_TRIANGLES, 0, vertexCount);

            myCube.render();
            myCube2.render();
            mySquare.render();

            glUniform1i(u_draw_ui, true);
//            glDisable(GL_DEPTH_TEST);
            glUniform1i(u_texture_sampler, 2);
            myText.render();

            r += increment;
            //endregion

            glfwSwapBuffers(window);
            glfwPollEvents();

        } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
                 glfwWindowShouldClose(window) == 0);
    }

    //region Termination
    glDeleteProgram(shader);
    glfwTerminate();
    //endregion

    return 0;
}
