//
// Created by vlado on 24-Jul-18.
//

#include <glfw3.h>
#include <iostream>
#include "GLInputHandler.h"

void GLInputHandler::processInput() const {
    for (const Handler &handler: handlers) {
        handler.handle();
    }
}

void GLInputHandler::onKey(int key, int state, const std::function<void()> &keyHandler) {
    Handler h([=]() {
        if (glfwGetKey(window, key) == state) {
            keyHandler();
        }
    });

    handlers.push_back(h);
}


void GLInputHandler::onMouse(int button, int state, const std::function<void()> &mouseButtonHandler) {
    Handler h([=]() {

        if (glfwGetMouseButton(window, button) == state) {
            mouseButtonHandler();
        }
    });

    handlers.push_back(h);
}

void GLInputHandler::onCursor(const std::function<void(double, double)> &cursorHandler) {
    Handler h([=]() {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        cursorHandler(xpos, ypos);
    });

    handlers.push_back(h);
}
