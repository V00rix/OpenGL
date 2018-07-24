//
// Created by vlado on 24-Jul-18.
//

#ifndef OPENGL_GLINPUT_H
#define OPENGL_GLINPUT_H

#include <utility>
#include <vector>
#include <functional>

class GLInputHandler {
private:
    GLFWwindow *window{};

    class Handler {
    protected:
        std::function<void()> handler;

    public:
        inline virtual void handle() const { handler(); };

        explicit Handler(std::function<void()> handler) : handler(std::move(handler)) {};
    };

    std::vector<Handler> handlers;

public:

    /**
     * Add key handler
     * @param key GLFW_KEY_*
     * @param state GLFW_* (press) state
     * @param keyHandler Handler function
     */
    void onKey(int key, int state, const std::function<void()> &keyHandler);

    /**
     * Add mouse button handler
     * @param button GLFW_MOUSE_BUTTON_*
     * @param state GLFW_* state
     * @param mouseButtonHandler Handler function
     */
    void onMouse(int button, int state, const std::function<void()> &mouseButtonHandler);

    /**
     * Add cursor movement handler
     * @param cursorHandler Handler
     */
    void onCursor(const std::function<void(double, double)> &cursorHandler);

    /**
     * Get all handlers
     * @return Pointer to handlers
     */
    inline Handler *getHandlers() { return &handlers[0]; };

    /**
     * Main events processing function
     */
    void processInput() const;

    explicit GLInputHandler(GLFWwindow *window) : window(window) {}
};


#endif //OPENGL_GLINPUT_H
