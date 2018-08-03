//
// Created by vlado on 24-Jul-18.
//

#ifndef OPENGL_GLCONTEXT_H
#define OPENGL_GLCONTEXT_H


#include <glfw3.h>
#include "../GLObject/GLObject.h"

/**
 * GLContext abstraction class
 */
class GLWindow : GLObject {
    /**
     *  Pointer to the current OpenGL initialized context
     *  Only one context can be initialized at runtime
     */
    static GLWindow *initialized;

    void initGLFW() const;

    void initGLEW() const;

    void createWindow();

    void printData() const override;

    void init();
public:
    GLWindow();
    explicit GLWindow(bool debug = false);
    GLWindow(unsigned width, unsigned height, bool debug = false);
    GLWindow(unsigned width, unsigned height, bool limitCursor = false, bool debug = false);
    GLWindow(unsigned width, unsigned height, const char * title, bool debug = false);

    virtual ~GLWindow();

    void cleanGL() const override;

    bool limitCursor{false};

    /**
     * OpenGL profile to create the context for
     */
    enum Profile {
        any = GLFW_OPENGL_ANY_PROFILE,
        core = GLFW_OPENGL_CORE_PROFILE,
        compatibility = GLFW_OPENGL_COMPAT_PROFILE
    } profile{any};

    /**
     * OpenGL version
     */
    const struct Version {
        unsigned major{4};
        unsigned minor{0};
    } version;

    /**
     *  Forward compatibility hint
     */
    const bool forwardCompatibility{true};

    /**
     *  Enable glew experimental
     *  @note needed for core profile
     */
    const bool experimental{true};

    /**
     *  Enable debug mode
     */
    const bool debug{false};

    /**
     *  Swap interval for the current OpenGL or OpenGL ES context,
     *  i.e. the number of screen updates to wait from the time
     *  glfwSwapBuffers was called before swapping the buffers and returning.
     *
     *
     */
    const union {
        /**
         * The minimum number of monitor refreshes between buffers swap
         */
        unsigned swapInterval;

        /**
         * Enable vertical synchronization and set swapInterval to 1
         * @see vsync
         */
        bool enabled;
    } vsync{1};

    /**
     *  Window data
     */
    GLFWwindow *window{nullptr};

    /**
     *  Window width
     */
    unsigned width{200};

    /**
     * Window height
     */
    unsigned height{100};

    /**
     * Window title
     */
    const char *const title{"OpenGL"};

    /**
     * Get currently initialized OpenGL context
     * @return Pointer to a context
     */
    inline GLWindow *getInitContext() const { return GLWindow::initialized; }
};


#endif //OPENGL_GLCONTEXT_H
