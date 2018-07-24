//
// Created by vlado on 24-Jul-18.
//

#ifndef OPENGL_GLCONTEXT_H
#define OPENGL_GLCONTEXT_H


#include <glfw3.h>

/**
 * GLContext abstraction class
 */
class GLWindow {
    /**
     *  Pointer to the current OpenGL initialized context
     *  Only one context can be initialized at runtime
     */
    static GLWindow *initialized;

    bool initGLFW();

    bool createWindow();

    bool initGLEW();

public:

    /**
     * OpenGL profile to create the context for
     */
    enum Profile {
        any = GLFW_OPENGL_ANY_PROFILE,
        core = GLFW_OPENGL_CORE_PROFILE,
        compatibility = GLFW_OPENGL_COMPAT_PROFILE
    } profile = any;


    /**
     *  Anti-aliasing
     */
    unsigned antiAliasing = 4;

    /**
     * OpenGL version
     */
    struct Version {
        unsigned major = 4;
        unsigned minor = 4;
    } version;

    /**
     *  Forward compatibility hint
     */
    bool forwardCompatibility = true;

    /**
     *  Enable glew experimental
     *  @note needed for core profile
     */
    bool experimental = true;

    /**
     *  Enable debug mode
     */
    bool debug = false;

    /**
     *  Swap interval for the current OpenGL or OpenGL ES context,
     *  i.e. the number of screen updates to wait from the time
     *  glfwSwapBuffers was called before swapping the buffers and returning.
     *
     *
     */
    union {
        /**
         * The minimum number of monitor refreshes between buffers swap
         */
        unsigned swapInterval = 1;

        /**
         * The minimum number of monitor refreshes between buffers swap
         */
        unsigned vsync;

        /**
         * Enable vertical synchronization and set swapInterval to 1
         * @see vsync
         */
        bool vsyncEnabled;
    };

    /**
     *  Window data
     */
    struct Window {
        /**
         *  Window width
         */
        unsigned width = 200;
        /**
         * Window height
         */
        unsigned height = 100;
        /**
         * Window title
         */
        const char *title = "OpenGL";

        GLFWwindow *ref = nullptr;
    } window;

    GLWindow();


    virtual ~GLWindow();

    /**
     * Close current OpenGL context
     */
    void close();

    /**
     * Initialize OpenGL context
     * @return Pointer to a context
     */
    GLWindow *init();

    /**
     * Get currently initialized OpenGL context
     * @return Pointer to a context
     */
    inline GLWindow *getInitContext() {
        return GLWindow::initialized;
    }
};


#endif //OPENGL_GLCONTEXT_H
